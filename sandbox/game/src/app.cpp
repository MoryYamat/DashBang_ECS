#include "game/app.h"

#include <cmath>
#include <iostream>

#include <ddknd/io/io.h>

#include "ddknd/camera/system_debug_camera.h"
#include "game/action/action.h"
#include <ddknd/camera/debug_camera.h>
#include <ddknd/clock/clock.h>
#include <ddknd/graphics/animation.h>
#include <ddknd/graphics/gfx_type.h>
#include <ddknd/graphics/renderer.h>
#include <ddknd/input/input.h>
#include <ddknd/window/window.h>

#include <ddknd/asset/asset_manager.h>
#include <ddknd/ecs/ecs.h>
#include <ddknd/graphics/gfx_asset_loader.h>

#include "game/system/game_system.h"
#include <ddknd/debug/debug_system.h>
#include <ddknd/system/system.h>

// event
#include <ddknd/event/hit_event.h>

// test
#include <ddknd/component/test_component.h>
#include <ddknd/ecs/entity/entity.h>
#include <ddknd/ecs/query/query.h>
#include <ddknd/ecs/registry/registry.h>
#include <ddknd/ecs/view/view.h>
#include <ddknd/ecs/world/world.h>

#include <ddknd/asset/asset_tag.h>

#include <ddknd/math/math.h>

#include <ddknd/component/gfx_component.h>
#include <ddknd/graphics/debug_animation.h>

#include <spdlog/spdlog.h>

#include "game/scene/game_scene.h"
#include "game/system/game_system.h"

// test font

// temporaly
namespace
{
    using Mat4f = ::ddknd::math::Mat4f;
    using Vec3f = ::ddknd::math::Vec3f;

    using Action = ::app::action::Action;
    using Key = ::ddknd::input::Key;

    // float MaxAbsDiffFromIdentity(const ::ddknd::math::Mat4f& m)
    // {
    //     const float* p = m.Data();
    //     const float* id = ::ddknd::math::Mat4f::Identity().Data();

    //     float maxDiff = 0.0f;
    //     for (int i = 0; i < 16; ++i)
    //         maxDiff = std::max(maxDiff, std::abs(p[i] - id[i]));

    //     return maxDiff;
    // }
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
        inputMapping_->RegisterKeyMap(Key::W, Action::MoveFoward);
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

        const bool loaded = app::scene::LoadMainSceneAssets(scene_->assets, loadCtx);
        if (!loaded)
        {
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
        // const auto font_res = graphicsAssetStore_->TryGet(font_res_id);

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
        Timer timer{};

        // ::ddknd::math::TRS modelTRS;
        // modelTRS.translation = {0.0f, 0.0f, 0.0f};
        // modelTRS.rotation = ::ddknd::math::Quatf::Identity();
        // modelTRS.scale = {0.01f, 0.01f, 0.01f};
        ::ddknd::graphics::RenderCamera frameCamera{};
        // ********* Debug Config ************
        ::ddknd::debug::DebugSystemRunner debugSystemRunner{};
        ::ddknd::debug::DebugCameraConfig debugCameraConfig{.overrideMode = ::ddknd::debug::CameraOverrideMode::None};
        ::ddknd::debug::DebugConfig debugConfig{.drawAxis = true, .drawFps = true, .drawSkeletons = true, .camera = debugCameraConfig};
        ::ddknd::debug::DebugDrawResources debugDrawResouces{.textShader = debug_font_shader_id, .lineShader = debug_line_shader_id, .font = font_res_id};
        // debugDraw_->SetFont(font_res);
        // debugDraw_->Axis({0.0f, 0.0f, 0.0f}, 1000.0f);
        while (isRunning_ && !window_->ShouldClose())
        {
            // ************ RESET ************
            hitboxHitEvents_->Clear();

            // ************* TIMER **************
            timer.Tick();
            const float fps = timer.FPS();

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
            ::ddknd::system::FrameContext frameCtx{.deltaTime = timer.DeltaTime(),
                                                   .actionInput = inputSys_.get(),
                                                   .aspect = window_->aspectRatio(),
                                                   .graphicsAssetStore = graphicsAssetStore_.get(),
                                                   .animationAssetStore = animationAssetStore_.get(),
                                                   .renderer = renderSys_.get(),
                                                   .renderCamera = &frameCamera,
                                                   .hitboxHitEvents = hitboxHitEvents_.get()};

            ::app::system::GameFrameContext gameCtx{.frame = &frameCtx, .input = inputSys_.get(), .paused = false};

            ::ddknd::debug::DebugContext debugCtx{.frame = &frameCtx, .debugDraw = debugDraw_.get(), .config = &debugConfig, .resources = &debugDrawResouces, .fps=timer.FPS()};
            // ************* BEGIN FRAME *************
            ddknd::graphics::FrameBeginDesc frameBegin{.h = window_->GetHeight(),
                                             .w = window_->GetWidth()};

            renderSys_->BeginFrame(frameBegin);

            gameSystemRunner_->UpdatePreEngine(*world_, gameCtx);
            engineSystemRunner_->UpdateSimulation(*world_, frameCtx);
            gameSystemRunner_->UpdatePostEngine(*world_, gameCtx);
            engineSystemRunner_->UpdateRenderPrepare(*world_, frameCtx);
            
            

            if(debugConfig.camera.overrideMode == ::ddknd::debug::CameraOverrideMode::DebugCamera)
            {
                // ************* DEBUG DRAW *************
                debugSystemRunner.BeginFrame(debugCtx);
                debugSystemRunner.Update(*world_, debugCtx);
                debugSystemRunner.EndFrame(debugCtx);
                debugSystemRunner.Submit(debugCtx);

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
        ::ddknd::ecs::Entity test{};

        ::ddknd::ecs::World wd{};

        // ======================= test for ecs systems =======================
        // auto ent0_0 = wd.Create();
        // auto ent0_1 = wd.Create();
        // auto ent0_2 = wd.Create();
        // auto ent0_3 = wd.Create();
        // auto ent0_4 = wd.Create();
        // auto ent0_5 = wd.Create();
        // auto ent0_6 = wd.Create();
        // auto ent0_7 = wd.Create();
        // auto ent0_8 = wd.Create();
        // auto ent0_9 = wd.Create();
        // auto ent0_10 = wd.Create();
        // auto ent0_11 = wd.Create();
        // auto ent0_12 = wd.Create();
        // auto ent0_13 = wd.Create();
        // auto ent0_14 = wd.Create();

        // wd.Destroy(ent0_0);
        // std::cerr << "3:" << wd.IsAlive(ent0_3) << "\n";
        // wd.Destroy(ent0_3);
        // std::cerr << "3:" << wd.IsAlive(ent0_3) << "\n";
        // wd.Destroy(ent0_14);
        // wd.Destroy(ent0_11);

        // auto ent1_1 = wd.Create();
        // auto ent1_3 = wd.Create();
        // auto ent1_14 = wd.Create();
        // auto ent1_11 = wd.Create();

        // std::cerr << "3:" << wd.IsAlive(ent0_3) << "\n";
        // std::cerr << "3:" << wd.IsAlive(ent1_3) << "\n";

        // // for test
        // using namespace ::ddknd::component;

        // Pos pos = wd.GetRegistry().AddComponent<Pos>(ent1_1, 1.0f, 1.0f, 1.0f);
        // wd.GetRegistry().AddComponent<Vel>(ent1_1, 1.0f, 1.0f, 1.0f);
        // Pos pos1_3 = wd.GetRegistry().AddComponent<Pos>(ent1_3, 1.0f, 1.0f, 1.0f);
        // wd.GetRegistry().AddComponent<Vel>(ent1_3, 1.0f, 1.0f, 1.0f);

        // Pos pos1_11 = wd.GetRegistry().AddComponent<Pos>(ent1_11, 1.0f, 1.0f, 1.0f);
        // wd.GetRegistry().AddComponent<Vel>(ent1_11, 1.0f, 1.0f, 1.0f);
        // wd.GetRegistry().AddComponent<Acc>(ent1_11, 1.0f, 1.0f, 1.0f);

        // std::cerr << "pos " << pos.x << " " << pos.y << " " << pos.z << "\n";
        // pos = wd.GetRegistry().AddComponent<Pos>(ent1_1, 2.0f, 2.0f, 2.0f);
        // std::cerr << "pos " << pos.x << " " << pos.y << " " << pos.z << "\n";
        // auto get1_1 = wd.GetRegistry().TryGetComponent<Pos>(ent1_1);
        // auto get1_3 = wd.GetRegistry().TryGetComponent<Pos>(ent1_3);
        // if (!get1_1)
        // {
        //     std::cerr << "null\n";
        // }
        // else
        // {
        //     std::cerr << "exists\n";
        // }

        // if (!get1_3)
        // {
        //     std::cerr << "null\n";
        // }
        // else
        // {
        //     std::cerr << "exists\n";
        // }

        // if (!wd.GetRegistry().HasComponent<Pos>(ent1_1))
        // {
        //     std::cerr << "not have component\n";
        // }
        // else
        // {
        //     wd.GetRegistry().RemoveComponent<Pos>(ent1_1);
        //     if (!wd.GetRegistry().HasComponent<Pos>(ent1_1))
        //         std::cerr << "not have component\n";
        // }
        // std::cerr << "=================== test for normal registry behavior ===================\n\n";

        // using namespace ::ddknd::query;

        // auto q_1 = query().select<Pos>().require<Vel>().exclude<Acc>(); // copy ctor
        // auto q_2 = query().select<Vel>().require<Pos>().exclude<Acc>(); // copy ctor

        // auto view_1 = wd.GetRegistry().view(q_1); // copy ctor
        // auto view_2 = wd.GetRegistry().view(q_2); // copy ctor

        // std::cerr << "view type=" << typeid(view_1).name() << "\n";
        // std::cerr << "view type=" << typeid(view_2).name() << "\n";

        // for (auto i = view_1.begin(); i != view_1.end(); ++i)
        // {
        //     std::cerr << "i " << i.idx << "\n";
        // }

        // std::cerr << "ent1_3 before compute " << pos1_3.x << " " << pos1_3.y << " " << pos1_3.z << "\n";

        // for (auto [pos, vel] : view_1)
        // {
        //     pos.x += vel.x;
        // }

        // auto pos1_3_ = wd.GetRegistry().GetComponent<Pos>(ent1_3);
        // std::cerr << "ent1_3 after compute " << pos1_3_.x << " " << pos1_3_.y << " " << pos1_3_.z << "\n";

        // auto view_3 = wd.GetRegistry().view(q_1).withEntity();

        // for (auto [e, pos, vel] : view_3)
        // {
        //     auto res = wd.GetRegistry().TryGetComponent<Pos>(e);
        //     if (res)
        //     {
        //         std::cerr << "get it\n";
        //     }
        // }
        // ======================= test for ecs systems =======================

        // ======================= test for input action systems =======================
        // using Key = ::ddknd::input::Key;
        // using Action = ::app::action::Action;
        // using InputMapping = ::ddknd::input::InputMapping;
        // using ActionInputSystem = ::ddknd::input::ActionInputSystem;

        // auto move_forward = inputMapping_->GetActionID(Action::MoveFoward);
        // auto move_left = inputMapping_->GetActionID(Action::MoveLeft);
        // auto move_backward = inputMapping_->GetActionID(Action::MoveBackward);
        // auto move_right = inputMapping_->GetActionID(Action::MoveRight);

        // assert(move_forward != InputMapping::InvalidID);
        // assert(move_left != InputMapping::InvalidID);
        // assert(move_backward != InputMapping::InvalidID);
        // assert(move_right != InputMapping::InvalidID);

        // assert(inputMapping_->GetActionFromKey(Key::W) == move_forward);
        // assert(inputMapping_->GetActionFromKey(Key::A) == move_left);
        // assert(inputMapping_->GetActionFromKey(Key::S) == move_backward);
        // assert(inputMapping_->GetActionFromKey(Key::D) == move_right);

        // assert(inputMapping_->GetKey(move_forward) == Key::W);
        // assert(inputMapping_->GetKey(move_left) == Key::A);
        // assert(inputMapping_->GetKey(move_backward) == Key::S);
        // assert(inputMapping_->GetKey(move_right) == Key::D);

        // // 未登録
        // assert(inputMapping_->GetActionID(static_cast<Action>(999)) == InputMapping::InvalidID);
        // assert(inputMapping_->GetActionFromKey(Key::F20) == InputMapping::InvalidID);
        // assert(inputMapping_->GetKey(InputMapping::InvalidID) == InputMapping::InvalidKey);
        // ======================= test for input action systems =======================
    }
} // namespace app

namespace
{
    // Mat4f LookAt(Vec3f eye, Vec3f target, Vec3f up)
    // {
    //     Vec3f f = normalize(target - eye);
    //     Vec3f s = normalize(cross(f, up)); // create fallback to avoid devide by 0
    //     Vec3f u = cross(s, f);

    //     Mat4f m{};

    //     m(0, 0) = s[0];
    //     m(0, 1) = s[1];
    //     m(0, 2) = s[2];
    //     m(0, 3) = -dot(s, eye);
    //     m(1, 0) = u[0];
    //     m(1, 1) = u[1];
    //     m(1, 2) = u[2];
    //     m(1, 3) = -dot(u, eye);
    //     m(2, 0) = -f[0];
    //     m(2, 1) = -f[1];
    //     m(2, 2) = -f[2];
    //     m(2, 3) = dot(f, eye);
    //     m(3, 0) = 0;
    //     m(3, 1) = 0;
    //     m(3, 2) = 0;
    //     m(3, 3) = 1;

    //     return m;
    // }

    // Mat4f Perspective(float fovY, float aspect, float near, float far)
    // {
    //     float f = 1.0f / std::tan(fovY * 0.5f);

    //     Mat4f m{};

    //     m(0, 0) = f / aspect;
    //     m(1, 1) = f;
    //     m(2, 2) = (far + near) / (near - far);
    //     m(2, 3) = (2 * far * near) / (near - far);
    //     m(3, 2) = -1.0f;

    //     return m;
    // }
} // namespace