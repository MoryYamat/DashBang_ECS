#pragma once

#include <ddknd/graphics/renderer.h>
#include <ddknd/input/input.h>
#include <ddknd/window/window_fwd.h>
#include <ddknd/input/input_fwd.h>
#include <ddknd/graphics/graphics_fwd.h>
#include <ddknd/io/io_fwd.h>
#include <ddknd/asset/asset_fwd.h>
#include <ddknd/ecs/ecs_fwd.h>
#include <ddknd/system/system_fwd.h>

#include "game/system/game_system_fwd.h"
#include "game/scene/game_scene_fwd.h"

#include <memory>

// fwd
namespace ddknd::component
{
    struct DebugCameraControllerComponent;
} 

namespace app
{
    class App
    {
        public:
            App();
            ~App();

            App(const App&) = delete;
            App& operator=(App&) = delete;

            App(App&&) = delete;
            App& operator=(App&&) = delete;

            bool Init(int w, int h);
            void Loop();
            void Shutdown();


        private:
            bool isRunning_ = false;
            std::unique_ptr<ddknd::window::Window> window_;
            std::unique_ptr<ddknd::window::GlfwContext> glfwCtx_;
            std::unique_ptr<ddknd::graphics::IRendererBackend> rendererBackend_;
            std::unique_ptr<ddknd::io::IPathResolver> vfs_;
            std::unique_ptr<ddknd::input::IInputBackend> inputBackend_;
            std::unique_ptr<ddknd::input::DeviceInput> deviceInput_;

            std::unique_ptr<ddknd::graphics::RendererSystem> renderSys_;

            std::unique_ptr<::ddknd::component::DebugCameraControllerComponent> debugCam_;

            std::unique_ptr<::ddknd::input::InputMapping> inputMapping_;
            std::unique_ptr<::ddknd::input::ActionInputSystem> inputSys_;

            std::unique_ptr<::ddknd::graphics::DebugDrawList> debugDraw_;

            // assets
            std::unique_ptr<::ddknd::asset::AssetManager> assetMgr_;
            std::unique_ptr<::ddknd::graphics::GraphicsAssetLoader> graphicsAssetLoader_;
            std::unique_ptr<::ddknd::graphics::GraphicsAssetStore> graphicsAssetStore_;
            std::unique_ptr<::ddknd::animation::AnimationAssetStore> animationAssetStore_;

            std::unique_ptr<::ddknd::ecs::World> world_;

            std::unique_ptr<::app::scene::GameScene> scene_;

            std::unique_ptr<::ddknd::system::EngineSystemRunner> engineSystemRunner_;
            std::unique_ptr<::app::system::GameSystemRunner> gameSystemRunner_;
    };
}// namespace app