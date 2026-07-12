#include "game/app.h"



#include <iostream>

// ************ engine ************
#include <ddknd/input/input.h>
#include <ddknd/window/window.h>
#include <ddknd/io/io.h>
#include <ddknd/clock/clock.h>
#include <ddknd/math/math.h>

#include <ddknd/graphics/animation.h>
#include <ddknd/graphics/gfx_type.h>
#include <ddknd/graphics/renderer.h>

#include <ddknd/camera/system_debug_camera.h>
#include <ddknd/camera/debug_camera.h>

#include <ddknd/asset/asset_tag.h>
#include <ddknd/asset/asset_manager.h>

#include <ddknd/graphics/gfx_asset_loader.h>

#include <ddknd/ecs/ecs.h>

#include <ddknd/debug/debug_system.h>
#include <ddknd/system/system.h>

// event
#include <ddknd/event/hit_event.h>


// component
#include <ddknd/component/gfx_component.h>
#include <ddknd/graphics/debug_animation.h>
#include <ddknd/component/test_component.h>

// ************ game ************ 
// Action Input
#include "game/action/action.h"

// system
#include "game/scene/game_scene.h"
#include "game/system/game_system.h"


// temporaly
namespace
{
    using Mat4f = ::ddknd::math::Mat4f;
    using Vec3f = ::ddknd::math::Vec3f;

    using Action = ::app::action::Action;
    using Key = ::ddknd::input::Key;

} // namespace

namespace app
{
    App::App() {}

    App::~App()
    {
        Shutdown();
    }

    bool App::Init(int w, int h)
    {
        glfwCtx_ = std::make_unique<ddknd::window::GlfwContext>();
        window_ = std::make_unique<ddknd::window::Window>(*glfwCtx_, w, h, "app");

        std::vector<::ddknd::io::VfsMount> mounts;
        mounts.push_back(ddknd::io::VfsMount{.scheme = "res", .root = "assets"});

        vfs_ = ddknd::io::CreateVfsResolver(mounts);

        inputBackend_ = ddknd::input::CreateGlfwInputBackend(*window_);
        deviceInput_ = std::make_unique<ddknd::input::DeviceInput>(*inputBackend_);

        // renderer backnend
        rendererBackend_ = ddknd::graphics::CreateOpenGLBackend(ddknd::graphics::OpenGLBackendDesc{});

        // renderer
        renderSys_ = std::make_unique<ddknd::graphics::RendererSystem>(*rendererBackend_);

        debugCam_ = std::make_unique<::ddknd::component::DebugCameraControllerComponent>();

        // user definition input
        inputMapping_ = std::make_unique<::ddknd::input::InputMapping>();
        inputSys_ = std::make_unique<::ddknd::input::ActionInputSystem>(inputMapping_.get());

        //@TODO: make factory class
        using Key = ::ddknd::input::Key;
        using MouseAxis = ::ddknd::input::MouseAxis;
        using MouseButton = ::ddknd::input::MouseButton;
        using Action = ::app::action::Action;
        inputMapping_->RegisterKeyMap(Key::W, Action::MoveForward);
        inputMapping_->RegisterKeyMap(Key::A, Action::MoveLeft);
        inputMapping_->RegisterKeyMap(Key::S, Action::MoveBackward);
        inputMapping_->RegisterKeyMap(Key::D, Action::MoveRight);
        inputMapping_->RegisterMouseAxisMap(MouseAxis::DeltaX, Action::CameraLookX);
        inputMapping_->RegisterMouseAxisMap(MouseAxis::DeltaY, Action::CameraLookY);
        inputMapping_->RegisterMouseAxisMap(MouseAxis::WheelY, Action::CameraZoom);
        inputMapping_->RegisterMouseButtonMap(MouseButton::LEFT_CLICK,Action::Attack);
        
        // Debug Draw
        debugDraw_ = std::make_unique<ddknd::graphics::DebugDrawList>(*rendererBackend_);
        debugDraw_->Init();

        // assets
        assetMgr_ = std::make_unique<::ddknd::asset::AssetManager>();
        graphicsAssetLoader_ = std::make_unique<::ddknd::graphics::GraphicsAssetLoader>(*vfs_, *rendererBackend_);
        graphicsAssetStore_ = std::make_unique<::ddknd::graphics::GraphicsAssetStore>();
        animationAssetStore_ = std::make_unique<::ddknd::animation::AnimationAssetStore>();

        // world
        world_ = std::make_unique<::ddknd::ecs::World>();

        // scene
        scene_ = std::make_unique<::app::scene::GameScene>(::app::scene::CreateMainScene(*world_, *assetMgr_));
        app::scene::SceneLoadContext loadCtx{.assetManager = assetMgr_.get(),
                                             .graphicsLoader = graphicsAssetLoader_.get(),
                                             .graphicsStore = graphicsAssetStore_.get(),
                                             .animationStore = animationAssetStore_.get()};
        // load all main scene assets
        const bool loaded = app::scene::LoadMainSceneAssets(scene_->assets, loadCtx);
        if (!loaded)
        {
            std::cerr << "Failed to load MainSceneAssets. \n";
            return false;
        }

        // events
        hitboxHitEvents_ = std::make_unique<ddknd::event::HitboxHitEventBuffer>();

        // system
        engineSystemRunner_ = std::make_unique<::ddknd::system::EngineSystemRunner>();

        return true;
    }

    void App::Loop()
    {
        isRunning_ = true;

        // ============= for test ==============
        // ========== AssetManager ==========
        using AssetManager = ::ddknd::asset::AssetManager;
        using ShaderTag = ::ddknd::asset::tag::Shader;
        using MeshTag = ::ddknd::asset::tag::Mesh;
        using ModelTag = ::ddknd::asset::tag::Model;
        using FontTag = ::ddknd::asset::tag::Font;

        // AssetManager asset_mgr;

        // debug font
        // shader
        auto debug_font_shader_id = assetMgr_->GetOrCreate<ShaderTag>("res://shaders/programs/debug_text.shader");
        auto loaded_res_shader_debug_font =
            graphicsAssetLoader_->LoadShader(*assetMgr_, *graphicsAssetStore_, debug_font_shader_id);
        // const auto debug_font_shader_res = graphicsAssetStore_->TryGet(debug_font_shader);
        // resource
        auto font_res_id = assetMgr_->GetOrCreate<FontTag>("res://fonts/NotoSans-VariableFont_wdth,wght.ttf");
        auto loaded_res_font_res = graphicsAssetLoader_->LoadFont(*assetMgr_, *graphicsAssetStore_, font_res_id);
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        // AssetStore の内部 storage が std::vector<T> のため
        // Set/resize によって capacity を超えると再配置が発生
        // 予め TryGet() で取得した T* が dangling pointer になる
        // 一時対策:
        //   - すべての Resource を Load した後に TryGet() する
        //   - または使用直前に TryGet() する
        // 原則:
        //   - 長期保持するのは AssetID
        //   - Resource* は短命な参照として扱う
        // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        // debug axis
        auto debug_line_shader_id = assetMgr_->GetOrCreate<ShaderTag>("res://shaders/programs/debug_line.shader");
        auto loaded_res_shader_debug_line =
            graphicsAssetLoader_->LoadShader(*assetMgr_, *graphicsAssetStore_, debug_line_shader_id);

        // TryGet()
        const auto debug_font_shader_res = graphicsAssetStore_->TryGet(debug_font_shader_id);
        const auto font_res = graphicsAssetStore_->TryGet(font_res_id);
        const auto debug_line_shader_res = graphicsAssetStore_->TryGet(debug_line_shader_id);

        assert(debug_font_shader_res);
        assert(font_res);
        assert(debug_line_shader_res);

        // DEBUG CAMERA
        using DebugCameraCtrl = ::ddknd::debug::DebugCameraController;
        ddknd::component::TransformComponent debug_camera_transform{};
        DebugCameraCtrl deug_cam(*deviceInput_, *debugCam_,
                                 debug_camera_transform); // @TODO change the target vector by the mouse moving
        debug_camera_transform.localTRS.translation = Vec3f{0.f, 0.f, 5.f};
        debugCam_->look.forward = Vec3f{0.f, 0.f, -1.f};
        debugCam_->yawDeg = -90.0f;
        debugCam_->pitchDeg = 0.0f;
        // ============= for test ==============

        // ************* TIMER *************
        using Timer = ::ddknd::clock::FrameTimer;
        using FrameTimeStatistics = ::ddknd::clock::FrameTimeStatistics;

        Timer timer{};
        FrameTimeStatistics frameTimeStatistics{};

        // ********* Debug Config ************
        ::ddknd::debug::DebugSystemRunner debugSystemRunner{};
        ::ddknd::debug::DebugCameraConfig debugCameraConfig{.overrideMode = ::ddknd::debug::CameraOverrideMode::None};
        ::ddknd::debug::DebugConfig debugConfig{.drawAxis = false,
                                                .drawFrameTimeInfo = true,
                                                .drawSkeletons = false,
                                                .drawHitboxes = false,
                                                .drawHurtboxes = false,
                                                .drawHitEvents = false,
                                                .camera = debugCameraConfig};
        ::ddknd::debug::DebugDrawResources debugDrawResouces{.textShader = debug_font_shader_id, .lineShader = debug_line_shader_id, .font = font_res_id};

        //  frame camera
        ::ddknd::graphics::RenderCamera frameCamera{};

        // game loop
        while (isRunning_ && !window_->ShouldClose())
        {
            // ************ RESET ************
            hitboxHitEvents_->Clear();

            // ************* TIMER **************
            timer.Tick();
            const float deltaTime = timer.DeltaTime();          // this is in seconds, not milliseconds
            // std::cerr << "deltatime" << deltaTime << "\n";
            if(deltaTime > 0)
            {
                frameTimeStatistics.PushFrame(deltaTime);
            }

            const float fps = timer.FPS();
            ddknd::debug::FramePerformanceInfo perf{.currentMs = frameTimeStatistics.CurrentFrameTimeMs(), 
                                               .p90Ms = frameTimeStatistics.P90FrameTimeMs(),
                                               .p99Ms = frameTimeStatistics.P99FrameTimeMs(),
                                               .fps = fps
                                            };


            // ************* Input *************
            deviceInput_->Update();
            inputSys_->Update(*deviceInput_.get());
            
            // std::cerr << "delta x: " << inputSys_->GetValue(Action::CameraLookX) << "\n";
            // std::cerr << "delta y: " << inputSys_->GetValue(Action::CameraLookY) << "\n";
            // std::cerr << "wheel y:" << inputSys_->GetValue(Action::CameraZoom) << "\n";
            
            // ************* CAMERA *************
            deug_cam.Update(timer.DeltaTime());
            ddknd::system::DebugCameraSystem::UpdateDebugCamera(debug_camera_transform, *debugCam_);

            // ************* FRAME CONTEXT *************
            ::ddknd::system::FrameContext frameCtx{.deltaTime = deltaTime,
                                                   .actionInput = inputSys_.get(),
                                                   .aspect = window_->aspectRatio(),
                                                   .graphicsAssetStore = graphicsAssetStore_.get(),
                                                   .animationAssetStore = animationAssetStore_.get(),
                                                   .renderer = renderSys_.get(),
                                                   .renderCamera = &frameCamera,
                                                   .hitboxHitEvents = hitboxHitEvents_.get()};

            ::app::system::GameFrameContext gameCtx{.frame = &frameCtx, .input = inputSys_.get(), .paused = false};

            ::ddknd::debug::DebugContext debugCtx{.frame = &frameCtx, .debugDraw = debugDraw_.get(), .config = &debugConfig, .resources = &debugDrawResouces, .framePerformance=perf};

            // ************* BEGIN FRAME *************
            ddknd::graphics::FrameBeginDesc frameBegin{.h = window_->GetHeight(),
                                             .w = window_->GetWidth()};

            renderSys_->BeginFrame(frameBegin);
            
            // Do not change the order.
            gameSystemRunner_->UpdatePreEngine(*world_, gameCtx);
            engineSystemRunner_->UpdateSimulation(*world_, frameCtx);
            gameSystemRunner_->UpdatePostEngine(*world_, gameCtx);
            engineSystemRunner_->UpdateRenderPrepare(*world_, frameCtx);
            
            
            // ************* DEBUG DRAW *************
            debugSystemRunner.BeginFrame(debugCtx);
            debugSystemRunner.Update(*world_, debugCtx);
            debugSystemRunner.EndFrame(debugCtx);
            debugSystemRunner.Submit(debugCtx);

            if(debugConfig.camera.overrideMode == ::ddknd::debug::CameraOverrideMode::DebugCamera)
            {
                // ************* DEBUG CAMERA *************
                frameCamera.view = debugCam_->matrices.view;
                frameCamera.proj = debugCam_->matrices.proj;
                frameCamera.valid = true;
            }

            // ************* END FRAME *************
            renderSys_->SetFrameCamera(frameCamera);
            renderSys_->EndFrame();

            // window_->PollEvents(); // moved to InputBackend
            window_->SwapBuffers();
            if (deviceInput_->isPressing(ddknd::input::Key::ESCAPE))
            {
                isRunning_ = false;
            }
        }
    }

    void App::Shutdown()
    {
    }
} // namespace app