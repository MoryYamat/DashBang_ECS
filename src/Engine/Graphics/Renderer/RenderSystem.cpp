#include "RenderSystem.h"

#include "Engine/ECS/Entity.h"


#include "Engine/ECS/Component/Graphics/MaterialComponent.h"
#include "Engine/ECS/Component/Graphics/AnimatorComponent.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#define MAX_BONES 128

// 現在未使用
//void Engine::Graphics::Render::UpdateRenderContext(Engine::ECS::EntityMgr& ecs, Engine::Graphics::Render::RenderContext& context)
//{
//	for (Engine::ECS::Entity e : ecs.view<
//		Engine::ECS::Component::Common::TransformComponent,
//		eNsCamComp::CameraComponent>())
//	{
//		const auto& transform = ecs.get<Engine::ECS::Component::Common::TransformComponent>(e);
//		const auto& cam = ecs.get<eNsCamComp::CameraComponent>(e);
//
//		// view / projection matrixの更新
//		glm::mat4 view = glm::lookAt(transform.position, transform.position + cam.front, cam.up);
//		glm::mat4 projection = glm::perspective(glm::radians(cam.fov), cam.aspect, cam.nearClip, cam.farClip);
//
//		// キャッシュ
//		context.cameraPosition = transform.position;
//		context.cameraFront = cam.front;
//		context.cameraRight = cam.right;
//		context.cameraUp = cam.up;
//
//		context.viewMatrix = view;
//		context.projectionMatrix = projection;
//
//		return;// 最初のカメラのみ使用
//	}
//
//	std::cerr << "[CameraSystem::UpdateRenderContext] : No camera found in ECS." << std::endl;
//}

// 一時ヘルパ
namespace
{
	inline void drawOneSubmesh(
		const Engine::ECS::Component::Graphics::MeshComponent& mc, size_t i
	)
	{
		const auto& mg = mc.modelGPU.meshesGPU[i];
		const auto& md = mc.modelData.meshes[i];
		glBindVertexArray(mg.vao);
		if (md.hasIndices) glDrawElements(GL_TRIANGLES, mg.indexCount, GL_UNSIGNED_INT, 0);
		else               glDrawArrays(GL_TRIANGLES, 0, mg.indexCount);
		glBindVertexArray(0);
	}
}

// Changed to update RenderContext
void Engine::Graphics::Render::RenderSystem(Engine::ECS::EntityMgr& ecs, Engine::Graphics::Render::Shader& shader, float aspect, RenderContext& context)
{
	// View行列、Projection行列
	glm::mat4 view, projection;

	if (!getCameraMatrices(ecs, view, projection, context))
	{
		std::cerr << "[RenderSystem.cpp]: No valid camera found in ECS." << std::endl;
		return;
	}



	for (Engine::ECS::Entity entity : ecs.view<
		Engine::ECS::Component::Common::TransformComponent, 
		Engine::ECS::Component::Graphics::MeshComponent, 
		Engine::ECS::Component::Graphics::MaterialComponent
	>())
	{
		auto& transformComp = ecs.get<Engine::ECS::Component::Common::TransformComponent>(entity);
		auto& meshComp = ecs.get<Engine::ECS::Component::Graphics::MeshComponent>(entity);
		auto& materialComp = ecs.get<Engine::ECS::Component::Graphics::MaterialComponent>(entity);
		

		// state machine (シェーダーを切り替えると、viewもprojectionもセットする必要あり。)
		shader.Use();
		shader.setMat4("uModel", transformComp.toMatrix());
		// shader.setMat4("uModel", glm::mat4(1.0f));
		shader.setMat4("uView", view);
		shader.setMat4("uProjection", projection);

		// ボーンパレットを送る
		int boneCount = 0;
		if (ecs.hasComponent<Engine::ECS::Component::Graphics::AnimatorComponent>(entity))
		{
			const auto& anim = ecs.get< Engine::ECS::Component::Graphics::AnimatorComponent>(entity);
			boneCount = (int)anim.palette.size();
			if (boneCount > 0)
			{
				boneCount = std::min(boneCount, MAX_BONES); // vs -> MAX_BONES
				shader.setInt("uBoneCount", boneCount);
				// shader.setInt("uBoneCount", 0);// -> スキニング側の問題っぽい

				GLint loc = glGetUniformLocation(shader.GetProgram(), "uBones[0]");
				if (loc == -1) {
					std::cerr << "[RenderSystem] Warning: uBones[0] uniform not found (maybe optimized out)" << std::endl;
				}
				else {
					glUniformMatrix4fv(loc, boneCount, GL_FALSE, glm::value_ptr(anim.palette[0]));
					// std::cout << "[RenderSystem] Uploaded bone palette: " << boneCount << " matrices" << std::endl;
				}
			}
			else
			{
				shader.setInt("uBoneCount", 0);
			}
		}
		else
		{
			// アニメータなし -> スキニングなし
			shader.setInt("uBoneCount", 0);
		}
		// meshごとに素材を反映
		for (size_t i = 0; i < meshComp.modelData.meshes.size(); ++i)
		{
			const auto& meshData = meshComp.modelData.meshes[i];

			bool hasTex = false;
			GLuint texId = 0;

			// textures[0] を "diffuse" として扱う
			if (!meshData.materialData.textures.empty())
			{
				texId = meshData.materialData.textures[0].id;
				hasTex = (texId != 0);
			}

			shader.setBool("uHasBaseColorTex", hasTex);
			if (hasTex)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texId);
				shader.setInt("uBaseColorTex", 0);// sampler2D に ユニット0 を関連付け
			}

			// テクスチャが無いとき用の係数
			shader.setVec3("uBaseColor", meshData.materialData.baseColor);

			// 1サブメッシュだけ描画するヘルパ
			drawOneSubmesh(meshComp, i);

			if (hasTex)
			{
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		//shader.setVec3("uBaseColor", materialComp.baseColor);
		// drawMesh(meshComp);
	}

	context.viewMatrix = view;
	context.projectionMatrix = projection;
	//context.viewport = {0, 0, WindowManager::GetWidth(), WindowManager::GetHeight()};
}

void Engine::Graphics::Render::drawMesh(const Engine::ECS::Component::Graphics::MeshComponent& meshComp)
{

	for (size_t i = 0; i < meshComp.modelData.meshes.size(); ++i)
	{
		const auto& meshGPU = meshComp.modelGPU.meshesGPU[i];
		const auto& meshData = meshComp.modelData.meshes[i];

		glBindVertexArray(meshGPU.vao);

		if (meshData.hasIndices)
		{
			glDrawElements(GL_TRIANGLES, meshGPU.indexCount, GL_UNSIGNED_INT, 0);
			//std::cout << "[RenderSystem.cpp]: draw elements" << std::endl;// for debugging
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, meshGPU.indexCount);
			//std::cout << "[RenderSystem.cpp]: draw Arrays" << std::endl;// for debugging
		}
	}

	glBindVertexArray(0);

	//glBindVertexArray(meshComp.modelGPU.meshesGPU[0].vao);
	//if (meshComp.modelData.meshes[0].hasIndices)
	//{
	//	glDrawElements(GL_TRIANGLES, meshComp.modelGPU.meshesGPU[0].indexCount, GL_UNSIGNED_INT, 0);
	//	//std::cout << "[RenderSystem.cpp]: draw elements" << std::endl;
	//}
	//else
	//{
	//	glDrawArrays(GL_TRIANGLES, 0, meshComp.modelGPU.meshesGPU[0].indexCount);
	//	//std::cout << "[RenderSystem.cpp]: draw Arrays" << std::endl;
	//}

}



bool Engine::Graphics::Render::getCameraMatrices(Engine::ECS::EntityMgr& ecs, glm::mat4& view, glm::mat4& projection)
{
	Engine::ECS::Component::Common::TransformComponent* camTransformComp = nullptr;
	Engine::ECS::Component::Camera::CameraComponent* camComp = nullptr;

	for (Engine::ECS::Entity entity : ecs.view<Engine::ECS::Component::Common::TransformComponent, Engine::ECS::Component::Camera::CameraComponent>())
	{
		camTransformComp = &ecs.get<Engine::ECS::Component::Common::TransformComponent>(entity);
		camComp = &ecs.get<Engine::ECS::Component::Camera::CameraComponent>(entity);
		break;
	}

	if (!camTransformComp || !camComp)
	{
		std::cerr << "[RenderSystem.cpp(getCameraMatrices)]: No Camera found!" << std::endl;
		return false;
	}

	view = computeViewMatrix(*camTransformComp, *camComp);
	projection = computeProjectionMatrix(camComp->fov, camComp->aspect, camComp->nearClip, camComp->farClip);

	return true;
}

bool Engine::Graphics::Render::getCameraMatrices(Engine::ECS::EntityMgr& ecs, glm::mat4& view, glm::mat4& projection, RenderContext& context)
{

	for (Engine::ECS::Entity entity : ecs.view<Engine::ECS::Component::Common::TransformComponent, Engine::ECS::Component::Camera::CameraComponent>())
	{
		const auto& camTransformComp = ecs.get<Engine::ECS::Component::Common::TransformComponent>(entity);
		const auto& camComp = ecs.get<Engine::ECS::Component::Camera::CameraComponent>(entity);

		view = computeViewMatrix(camTransformComp, camComp);
		projection = computeProjectionMatrix(camComp.fov, camComp.aspect, camComp.nearClip, camComp.farClip);

		context.cameraPosition = camTransformComp.position;
		context.cameraFront = camComp.front;
		context.cameraRight = camComp.right;
		context.cameraUp = camComp.up;

		return true;

	}


	// カメラが見つからなかった
	std::cerr << "[RenderSystem.cpp(getCameraMatrices)]: No Camera found!" << std::endl;

	return false;
}



glm::mat4 Engine::Graphics::Render::computeViewMatrix(const Engine::ECS::Component::Common::TransformComponent& transformComp, const Engine::ECS::Component::Camera::CameraComponent& cameraComp)
{
	glm::vec3 position = transformComp.position;
	return glm::lookAt(position, position + cameraComp.front, cameraComp.up);
}

glm::mat4 Engine::Graphics::Render::computeProjectionMatrix(float fov, float aspect, float nearClip, float farClip)
{
	return glm::perspective(fov, aspect, nearClip, farClip);
}



// For simple testing
void Engine::Graphics::Render::RenderSystem(Engine::ECS::EntityMgr& ecs, Engine::Graphics::Render::Shader& shader, float aspect)
{
	//for (auto entity : ecs.view<MeshComponent>())// for Test
	//{
	//	//auto& transformComp = ecs.get<TransformComponent>(entity);
	//	auto& meshComp = ecs.get<MeshComponent>(entity);
	// 
	//	drawMesh(meshComp);
	//}
	glm::mat4 view, projection;

	if (!getCameraMatrices(ecs, view, projection))
	{
		std::cerr << "[RenderSystem.cpp]: No valid camera found in ECS." << std::endl;
		return;
	}



	for (Engine::ECS::Entity entity : ecs.view<Engine::ECS::Component::Common::TransformComponent, Engine::ECS::Component::Graphics::MeshComponent>())
	{
		auto& transformComp = ecs.get<Engine::ECS::Component::Common::TransformComponent>(entity);
		auto& meshComp = ecs.get<Engine::ECS::Component::Graphics::MeshComponent>(entity);


		// state machine (シェーダーを切り替えると、viewもprojectionもセットする必要あり。)
		shader.Use();
		shader.setMat4("uModel", transformComp.toMatrix());
		shader.setMat4("uView", view);
		shader.setMat4("uProjection", projection);
		drawMesh(meshComp);
	}

}