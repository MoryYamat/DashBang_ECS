#include "app.h"

#include <iostream>

#include <io/io.h>

#include "asset/asset.h"
#include "graphics/renderer.h"
#include "input/input.h"
#include "window/window.h"


// test
#include "component/test_component.h"
#include "entity/entity.h"
#include "query/query.h"
#include "registry/registry.h"
#include "view/view.h"
#include "world/world.h"


#include "math/math.h"

#include <spdlog/spdlog.h>

namespace app
{
    App::App(int w, int h) : w_(w), h_(h) {}

    App::~App()
    {
        Shutdown();
    }

    bool App::Init()
    {
        glfwCtx_ = std::make_unique<ddknd::window::GlfwContext>();
        window_ = std::make_unique<ddknd::window::Window>(*glfwCtx_, w_, h_, "app");

        std::vector<::ddknd::io::VfsMount> mounts;
        mounts.push_back(ddknd::io::VfsMount{.scheme = "res", .root = "assets"});

        vfs_ = ddknd::io::CreateVfsResolver(mounts);

        inputBackend_ = ddknd::input::CreateGlfwInputBackend(*window_);
        inputSys_ = std::make_unique<ddknd::input::InputSystem>(*inputBackend_);

        // renderer backnend
        rendererBackend_ = ddknd::graphics::CreateOpenGLBackend(ddknd::graphics::OpenGLBackendDesc{});

        // asset manager
        assetMgr_ = std::make_unique<ddknd::asset::AssetManager>(*vfs_, *rendererBackend_);

        // shader
        shaderHandle_ =
            assetMgr_->GetOrCreate<ddknd::asset::type::ShaderResource>("res://shaders/programs/test.shader");

        auto okShader = assetMgr_->LoadNowShader(shaderHandle_.Id());
        spdlog::info("LoadNowShader: {}", okShader);

        // mesh
        meshHandle_ = assetMgr_->GetOrCreate<ddknd::asset::type::MeshResource>("res://meshes/test_triangle.mesh");

        const bool okMesh = assetMgr_->LoadNowMesh(meshHandle_.Id());
        spdlog::info("LoadNowMesh: {}", okMesh);

        // renderer
        renderSys_ = std::make_unique<ddknd::graphics::RendererSystem>(*rendererBackend_);

        return true;
    }

    void App::Loop()
    {
        isRunning_ = true;
        while (isRunning_ && !window_->ShouldClose())
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
                    .mesh = mesh_test->mesh, .shader = shader_test->program, .vcount = mesh_test->vertexCount});
            }

            if (!mesh_test)
                spdlog::error("mesh is nullptr");

            if (!shader_test)
                spdlog::error("shader is nullptr");

            inputSys_->Update();
            window_->PollEvents();
            window_->SwapBuffers();
            if (inputSys_->isPressing(ddknd::input::Key::Escape))
                isRunning_ = false;
        }
    }

    void App::Shutdown()
    {
        ::ddknd::entity::Entity test{};

        ::ddknd::world::World wd{};

        auto ent0_0 = wd.Create();
        auto ent0_1 = wd.Create();
        auto ent0_2 = wd.Create();
        auto ent0_3 = wd.Create();
        auto ent0_4 = wd.Create();
        auto ent0_5 = wd.Create();
        auto ent0_6 = wd.Create();
        auto ent0_7 = wd.Create();
        auto ent0_8 = wd.Create();
        auto ent0_9 = wd.Create();
        auto ent0_10 = wd.Create();
        auto ent0_11 = wd.Create();
        auto ent0_12 = wd.Create();
        auto ent0_13 = wd.Create();
        auto ent0_14 = wd.Create();

        wd.Destroy(ent0_0);
        std::cerr << "3:" << wd.IsAlive(ent0_3) << "\n";
        wd.Destroy(ent0_3);
        std::cerr << "3:" << wd.IsAlive(ent0_3) << "\n";
        wd.Destroy(ent0_14);
        wd.Destroy(ent0_11);

        auto ent1_1 = wd.Create();
        auto ent1_3 = wd.Create();
        auto ent1_14 = wd.Create();
        auto ent1_11 = wd.Create();

        std::cerr << "3:" << wd.IsAlive(ent0_3) << "\n";
        std::cerr << "3:" << wd.IsAlive(ent1_3) << "\n";

        // for test
        using namespace ::ddknd::component;

        Pos pos = wd.GetRegistry().AddComponent<Pos>(ent1_1, 1.0f, 1.0f, 1.0f);
        wd.GetRegistry().AddComponent<Vel>(ent1_1, 1.0f, 1.0f, 1.0f);
        Pos pos1_3 = wd.GetRegistry().AddComponent<Pos>(ent1_3, 1.0f, 1.0f, 1.0f);
        wd.GetRegistry().AddComponent<Vel>(ent1_3, 1.0f, 1.0f, 1.0f);

        Pos pos1_11 = wd.GetRegistry().AddComponent<Pos>(ent1_11, 1.0f, 1.0f, 1.0f);
        wd.GetRegistry().AddComponent<Vel>(ent1_11, 1.0f, 1.0f, 1.0f);
        wd.GetRegistry().AddComponent<Acc>(ent1_11, 1.0f, 1.0f, 1.0f);

        std::cerr << "pos " << pos.x << " " << pos.y << " " << pos.z << "\n";
        pos = wd.GetRegistry().AddComponent<Pos>(ent1_1, 2.0f, 2.0f, 2.0f);
        std::cerr << "pos " << pos.x << " " << pos.y << " " << pos.z << "\n";
        auto get1_1 = wd.GetRegistry().TryGetComponent<Pos>(ent1_1);
        auto get1_3 = wd.GetRegistry().TryGetComponent<Pos>(ent1_3);
        if (!get1_1)
        {
            std::cerr << "null\n";
        }
        else
        {
            std::cerr << "exists\n";
        }

        if (!get1_3)
        {
            std::cerr << "null\n";
        }
        else
        {
            std::cerr << "exists\n";
        }

        if (!wd.GetRegistry().HasComponent<Pos>(ent1_1))
        {
            std::cerr << "not have component\n";
        }
        else
        {
            wd.GetRegistry().RemoveComponent<Pos>(ent1_1);
            if (!wd.GetRegistry().HasComponent<Pos>(ent1_1))
                std::cerr << "not have component\n";
        }
        std::cerr << "=================== test for normal registry behavior ===================\n\n";

        using namespace ::ddknd::query;

        auto q_1 = query().select<Pos>().require<Vel>().exclude<Acc>(); // copy ctor
        auto q_2 = query().select<Vel>().require<Pos>().exclude<Acc>(); // copy ctor

        auto view_1 = wd.GetRegistry().view(q_1); // copy ctor
        auto view_2 = wd.GetRegistry().view(q_2); // copy ctor

        std::cerr << "view type=" << typeid(view_1).name() << "\n";
        std::cerr << "view type=" << typeid(view_2).name() << "\n";

        for (auto i = view_1.begin(); i != view_1.end(); ++i)
        {
            std::cerr << "i " << i.idx << "\n";
        }

        std::cerr << "ent1_3 before compute " << pos1_3.x << " " << pos1_3.y << " " << pos1_3.z << "\n";

        for (auto [pos, vel] : view_1)
        {
            pos.x += vel.x;
        }

        auto pos1_3_ = wd.GetRegistry().GetComponent<Pos>(ent1_3);
        std::cerr << "ent1_3 after compute " << pos1_3_.x << " " << pos1_3_.y << " " << pos1_3_.z << "\n";

        auto view_3 = wd.GetRegistry().view(q_1).withEntity();

        for (auto [e, pos, vel] : view_3)
        {
            auto res = wd.GetRegistry().TryGetComponent<Pos>(e);
            if (res)
            {
                std::cerr << "get it\n";
            }
        }

        auto q_3 = query().require<Pos>().exclude<Acc>();

        std::cerr << "min loop " << typeid(q_3).name() << "\n";

        using Mat4f = ::ddknd::math::Mat<float, 4, 4>;

        Mat4f a = {Mat4f::Identity()};
        Mat4f b = {Mat4f::Identity()};
        b(0,0) = 2;

        Mat4f c = a * b;

        std::cerr << c << "\n";
    }
} // namespace app