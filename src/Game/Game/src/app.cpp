#include "app.h"

#include "window/window.h"
#include "input/input.h"
#include "asset/asset.h"
#include "graphics/renderer.h"

#include <io/io.h>

#include <spdlog/spdlog.h>

namespace app
{
    App::App(int w, int h) : w_(w), h_(h)
    {

    }

    App::~App()
    {

    }

    bool App::Init()
    {
        glfwCtx_ = std::make_unique<ddknd::window::GlfwContext>();
        window_ = std::make_unique<ddknd::window::Window>(*glfwCtx_, w_, h_, "app");

  		std::vector<::ddknd::io::VfsMount> mounts;
		mounts.push_back(ddknd::io::VfsMount{ .scheme = "res", .root = "assets" });

		vfs_ = ddknd::io::CreateVfsResolver(mounts);

        inputBackend_ = ddknd::input::CreateGlfwInputBackend(*window_);
        inputSys_ = std::make_unique<ddknd::input::InputSystem>(*inputBackend_);

        // renderer backnend
		rendererBackend_ = ddknd::graphics::CreateOpenGLBackend(ddknd::graphics::OpenGLBackendDesc{});

        // asset manager
		assetMgr_ = std::make_unique<ddknd::asset::AssetManager>(*vfs_, *rendererBackend_);


		// shader
		shaderHandle_ = assetMgr_->GetOrCreate<ddknd::asset::type::ShaderResource>(
			"res://shaders/programs/test.shader"
		);

		auto okShader = assetMgr_->LoadNowShader(shaderHandle_.Id());
		spdlog::info("LoadNowShader: {}", okShader);

		// mesh
		meshHandle_ = assetMgr_->GetOrCreate<ddknd::asset::type::MeshResource>(
			"res://meshes/test_triangle.mesh"
		);

		const bool okMesh= assetMgr_->LoadNowMesh(meshHandle_.Id());
		spdlog::info("LoadNowMesh: {}", okMesh);

        
        // renderer
        renderSys_ = std::make_unique<ddknd::graphics::RendererSystem>(*rendererBackend_);

        return true;
    }

    void App::Loop()
    {
        isRunning_ = true;
        while(isRunning_ && !window_->ShouldClose())
        {
            ddknd::graphics::FrameDesc frame{.h = h_, .w = w_};
            renderSys_->BeginFrame(frame);
            renderSys_->EndFrame();

            auto* mesh_test = assetMgr_->TryGet<ddknd::asset::type::MeshResource>(meshHandle_.Id());
            auto* shader_test = assetMgr_->TryGet<ddknd::asset::type::ShaderResource>(shaderHandle_.Id());

            			// send command to renderer
			if (mesh_test && shader_test)
			{
				renderSys_->DrawTestTriangle(ddknd::graphics::TestDrawTriangleCommand{
					.mesh = mesh_test->mesh, 
					.shader = shader_test->program, 
					.vcount = mesh_test->vertexCount});
			}
            
			if (!mesh_test)
				spdlog::error("mesh is nullptr");

			if (!shader_test)
				spdlog::error("shader is nullptr");

            inputSys_->Update();
            window_->PollEvents();
            window_->SwapBuffers();
            if(inputSys_->isPressing(ddknd::input::Key::Escape))
                isRunning_ =false;
        }
    }

    void App::Shutdown()
    {

    }

} // namespace app