# ecs 

ecs 開発時の検証用コードの記録

```cpp
    void RegistryTest::RunRegistryTest()
    {
        // ======================= test for ecs systems =======================
        ::ddknd::ecs::Entity test{};
        ::ddknd::ecs::World wd{};

        // ********* testing Entity creation and destruction  *********
        const auto& reg = wd.GetRegistry();
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
        assert(wd.IsAlive(ent0_3));
        wd.Destroy(ent0_3);
        assert(!wd.IsAlive(ent0_3));

        wd.Destroy(ent0_14);
        wd.Destroy(ent0_11);

        auto ent1_1 = wd.Create();
        auto ent1_3 = wd.Create();
        auto ent1_14 = wd.Create();
        auto ent1_11 = wd.Create();

        assert(!wd.IsAlive(ent0_3));
        assert(wd.IsAlive(ent1_3));


        // ********* get and compute component tests *********
        using namespace ddknd::tests::component;

        Pos pos = wd.GetRegistry().AddComponent<Pos>(ent1_1, 1.0f, 1.0f, 1.0f);
        wd.GetRegistry().AddComponent<Vel>(ent1_1, 1.0f, 1.0f, 1.0f);
        Pos pos1_3 = wd.GetRegistry().AddComponent<Pos>(ent1_3, 1.0f, 1.0f, 1.0f);
        wd.GetRegistry().AddComponent<Vel>(ent1_3, 1.0f, 1.0f, 1.0f);

        Pos pos1_11 = wd.GetRegistry().AddComponent<Pos>(ent1_11, 1.0f, 1.0f, 1.0f);
        wd.GetRegistry().AddComponent<Vel>(ent1_11, 1.0f, 1.0f, 1.0f);
        wd.GetRegistry().AddComponent<Acc>(ent1_11, 1.0f, 1.0f, 1.0f);

        assert(pos.x == 1.0f && pos.y == 1.0f && pos.z == 1.0f);
        pos = wd.GetRegistry().AddComponent<Pos>(ent1_1, 2.0f, 2.0f, 2.0f);
        assert(pos.x == 2.0f && pos.y == 2.0f && pos.z == 2.0f);

        assert(wd.GetRegistry().TryGetComponent<Pos>(ent1_1));
        assert(wd.GetRegistry().HasComponent<Pos>(ent1_1));

        assert(wd.GetRegistry().TryGetComponent<Pos>(ent1_3));
        assert(wd.GetRegistry().TryGetComponent<Pos>(ent1_1));

        // std::cerr << "=================== test for normal registry behavior ===================\n\n";
        using namespace ::ddknd::ecs;

        // ********* query-type test *********
        auto q_1 = query().select<Pos>().require<Vel>().exclude<Acc>(); // copy ctor
        auto q_2 = query().select<Vel>().require<Pos>().exclude<Acc>(); // copy ctor

        auto view_1 = wd.GetRegistry().view(q_1); // copy ctor
        auto view_2 = wd.GetRegistry().view(q_2); // copy ctor

        static_assert(
            std::is_same_v<decltype(view_1),
                           ddknd::ecs::View<ddknd::ecs::Query<ddknd::tests::component::Pos,
                                                              ddknd::ecs::TypeList<ddknd::tests::component::Vel>,
                                                              ddknd::ecs::TypeList<ddknd::tests::component::Acc>>>>);
        static_assert(
            std::is_same_v<decltype(view_2),
                           ddknd::ecs::View<ddknd::ecs::Query<ddknd::tests::component::Vel,
                                                              ddknd::ecs::TypeList<ddknd::tests::component::Pos>,
                                                              ddknd::ecs::TypeList<ddknd::tests::component::Acc>>>>);

        // ********* compute result validation *********
        for (auto [pos, vel] : view_1)
        {
            pos.x += vel.x;
        }
        auto pos1_3_after = wd.GetRegistry().GetComponent<Pos>(ent1_3);
        assert(pos1_3_after.x == 2 && pos1_3_after.y == 1 && pos1_3_after.z == 1);

        // ********* test of the .withEinty() *********
        auto view_3 = wd.GetRegistry().view(q_1).withEntity();
        for (auto [e, pos, vel] : view_3)
        {
            assert(wd.GetRegistry().TryGetComponent<Pos>(e));
        }
        // ======================= test for ecs systems =======================
    }
```