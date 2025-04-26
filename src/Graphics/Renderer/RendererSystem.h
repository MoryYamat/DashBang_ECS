// Rendering system

#include "Core/ECS/EntityManager.h"

#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/MeshComponent.h"

#include "Graphics/Renderer/Shader.h"

namespace RenderSystem
{
	void RenderSystem(ECS& ecs, Shader& shader, float aspect);


	void drawMesh(const MeshComponent& meshComp);

}
