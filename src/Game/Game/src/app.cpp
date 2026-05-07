#include "app.h"

#include <cmath>
#include <iostream>

#include <io/io.h>

#include "Action/action.h"
#include "camera/debug_camera.h"
#include "graphics/renderer.h"
#include "input/input.h"
#include "window/window.h"

// test
#include "component/test_component.h"
#include "ecs/entity/entity.h"
#include "ecs/query/query.h"
#include "ecs/registry/registry.h"
#include "ecs/view/view.h"
#include "ecs/world/world.h"

#include "asset/asset_manager.h"
#include "asset/asset_tag.h"

#include "graphics/gfx_asset_loader.h"

#include "math/math.h"

#include <spdlog/spdlog.h>

// temporaly
namespace
{
    using Mat4f = ::ddknd::math::Mat4f;
    using Vec3f = ::ddknd::math::Vec3f;
    Mat4f LookAt(Vec3f eye, Vec3f target, Vec3f up);

    Mat4f Perspective(float fovY, float aspect, float near, float far);

    using Action = ::app::action::Action;
    using Key = ::ddknd::input::Key;



} // namespace

namespace app
{
    App::App(){}

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

        cam_ = std::make_unique<::ddknd::component::DebugCameraComponent>();

        // user definition input 
        inputMapping_ = std::make_unique<::ddknd::input::InputMapping>();
        inputSys_ = std::make_unique<::ddknd::input::ActionInputSystem>(inputMapping_.get());

        //@TODO: make factory class
        using Key = ::ddknd::input::Key;
        using Action = ::app::action::Action;
        inputMapping_->RegisterKeyMap(Key::W, Action::MoveFoward);
        inputMapping_->RegisterKeyMap(Key::A, Action::MoveLeft);
        inputMapping_->RegisterKeyMap(Key::S, Action::MoveBackward);
        inputMapping_->RegisterKeyMap(Key::D, Action::MoveRight);

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

        AssetManager asset_mgr;

        auto res_1 = asset_mgr.GetOrCreate<ShaderTag>("res://shaders/programs/test.shader");
        auto res_2 = asset_mgr.GetOrCreate<MeshTag>("res://meshes/test_triangle.mesh");
        auto mod_1 = asset_mgr.GetOrCreate<ModelTag>("res://Models/paladin/base_action_animation_diago.glb");

        std::cerr << "id1: Idx=" << res_1.Index() << " Gen=" << res_1.Generation() << "\n";
        std::cerr << "id2: Idx=" << res_2.Index() << " Gen=" << res_2.Generation() << "\n";

        using GraphicsAssetStore = ::ddknd::graphics::GraphicsAssetStore;
        using AnimationAssetStore = ::ddknd::animation::AnimationAssetStore;
        GraphicsAssetStore gfx_asset_store;
        AnimationAssetStore gfx_anim_store;

        using GraphicsAssetLoader = ::ddknd::graphics::GraphicsAssetLoader;
        GraphicsAssetLoader gfx_loader(*vfs_, *rendererBackend_);

        auto load_res_gfx = gfx_loader.LoadShader(asset_mgr, gfx_asset_store, res_1);
        auto load_mod_gfx = gfx_loader.LoadModel(asset_mgr, gfx_asset_store, gfx_anim_store, mod_1);

        const auto* shader_res = gfx_asset_store.TryGet(res_1);
        const auto* model_res = gfx_asset_store.TryGet(mod_1);


        using DrawCommand = ::ddknd::graphics::DrawCommand;
        // DrawCommand cmd{.shader=shader_res->program};

        // INPUT
        using Action = ::app::action::Action;

        // DEBUG CAMERA
        using DebugCameraCtrl = ::ddknd::debug::DebugCameraController;
        DebugCameraCtrl deug_cam(*deviceInput_, *cam_);// @TODO change the target vector by the mouse moving
        cam_->pos = {0.0f, 2.0f, 5.0f};
        // ============= for test ==============

        while (isRunning_ && !window_->ShouldClose())
        {
            cam_->aspect = window_->aspectRation();
            // =========================== temporaly ===========================
            auto view = LookAt(cam_->pos, cam_->target, cam_->up);
            auto proj = Perspective(::ddknd::math::degToRadf(60.0f), cam_->aspect, 0.1f, 100.0f);
            // =========================== temporaly =========================== 

            ddknd::graphics::FrameDesc frame{.h = window_->GetHeight(), .w = window_->GetWidth(), .view = view, .proj = proj};
            renderSys_->BeginFrame(frame);

            for (const auto& res : model_res->primitives)
            {
                // std::cerr << "prim_id=" << res.prim.Value() << "\n";
                renderSys_->Submit(
                    DrawCommand{.mesh = res.prim, .shader = shader_res->program, .indexCount = res.indexCount});
            }

            renderSys_->EndFrame();


            deviceInput_->Update();

            // ================== Debug Camera ================== 
            deug_cam.Update();

            // ================== Update Action Input ================== 
            inputSys_->Update(*deviceInput_.get());
            
            //window_->PollEvents(); // moved to InputBackend
            window_->SwapBuffers();
            if (deviceInput_->isPressing(ddknd::input::Key::ESCAPE))
                isRunning_ = false;
        }
    }

    void App::Shutdown()
    {
        ::ddknd::entity::Entity test{};

        ::ddknd::world::World wd{};

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
        using Key = ::ddknd::input::Key;
        using Action = ::app::action::Action;
        using InputMapping = ::ddknd::input::InputMapping;
        using ActionInputSystem = ::ddknd::input::ActionInputSystem;
        
        auto move_forward = inputMapping_->GetActionID(Action::MoveFoward);
        auto move_left = inputMapping_->GetActionID(Action::MoveLeft);
        auto move_backward = inputMapping_->GetActionID(Action::MoveBackward);
        auto move_right = inputMapping_->GetActionID(Action::MoveRight);

        assert(move_forward != InputMapping::InvalidID);
        assert(move_left != InputMapping::InvalidID);
        assert(move_backward != InputMapping::InvalidID);
        assert(move_right != InputMapping::InvalidID);

        assert(inputMapping_->GetActionFromKey(Key::W) == move_forward);
        assert(inputMapping_->GetActionFromKey(Key::A) == move_left);
        assert(inputMapping_->GetActionFromKey(Key::S) == move_backward);
        assert(inputMapping_->GetActionFromKey(Key::D) == move_right);

        assert(inputMapping_->GetKey(move_forward) == Key::W);
        assert(inputMapping_->GetKey(move_left) == Key::A);
        assert(inputMapping_->GetKey(move_backward) == Key::S);
        assert(inputMapping_->GetKey(move_right) == Key::D);

        // 未登録
        assert(inputMapping_->GetActionID(static_cast<Action>(999)) == InputMapping::InvalidID);
        assert(inputMapping_->GetActionFromKey(Key::F20) == InputMapping::InvalidID);
        assert(inputMapping_->GetKey(InputMapping::InvalidID) == InputMapping::InvalidKey);
        // ======================= test for input action systems ======================= 
        
    }
} // namespace app

namespace
{
    Mat4f LookAt(Vec3f eye, Vec3f target, Vec3f up)
    {
        Vec3f f = normalize(target - eye);
        Vec3f s = normalize(cross(f, up));// create fallback to avoid devide by 0
        Vec3f u = cross(s, f);

        Mat4f m{};

        m(0, 0) = s[0];
        m(0, 1) = s[1];
        m(0, 2) = s[2];
        m(0, 3) = -dot(s, eye);
        m(1, 0) = u[0];
        m(1, 1) = u[1];
        m(1, 2) = u[2];
        m(1, 3) = -dot(u, eye);
        m(2, 0) = -f[0];
        m(2, 1) = -f[1];
        m(2, 2) = -f[2];
        m(2, 3) = dot(f, eye);
        m(3, 0) = 0;
        m(3, 1) = 0;
        m(3, 2) = 0;
        m(3, 3) = 1;

        return m;
    }

    Mat4f Perspective(float fovY, float aspect, float near, float far)
    {
        float f = 1.0f / std::tan(fovY * 0.5f);

        Mat4f m{};

        m(0, 0) = f / aspect;
        m(1, 1) = f;
        m(2, 2) = (far + near) / (near - far);
        m(2, 3) = (2 * far * near) / (near - far);
        m(3, 2) = -1.0f;

        return m;
    }
} // namespace