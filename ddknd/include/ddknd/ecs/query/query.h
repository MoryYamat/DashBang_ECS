#pragma once

namespace ddknd::ecs
{
    // Query specific entities that satisfy the specified constraints.
    //

    // ========================== NEW =============================
    // The query primarily determines the plan for how and what to scan the storage for
    // Independence from the world
    // API:
    // 
    // Clear indication of the main subject:
    // query().select<Position>()
    //        .require<Velocity>()
    //        .exclude<Disabled>()
    // @TODO:
    // Minimum scan:
    // query().require<C1>()
    //        .exclude<C2>()
    template <typename... Ts>
    struct TypeList
    {
    };

    struct None
    {
    };

    template <typename S, typename R, typename E>
    struct Query
    {
        using selected = S;
        using requiredList = R;
        using excludedList = E;
        Query() = default;

        template <typename T>
        auto select() const
        {
            return Query<T, R, E>{};
        }

        template <typename... Ts>
        auto require() const
        {
            return Query<S, TypeList<Ts...>, E>{};
        }

        template <typename... Ts>
        auto exclude() const
        {
            return Query<S, R, TypeList<Ts...>>{};
        }
    };

    // factory
    inline auto query()
    {
        return Query<None, TypeList<>, TypeList<>>{};
    }



    // =========================== OLD =============================
    // =============================================================
    // ## Problems
    // - Entityを返すAPI(既存`ForEach`)はユーザにAoS的使用を強制する
    // -
    // (storageの課題(改善済み))entityの格納が密(span)でも、実データはヒープに分散しているのでキャッシュの空間局所性が低い(ECSのメリットが半減)
    // ## needs
    // -
    // SoA/AoSの実データのデータ構造に依存せず、ユーザが欲しい形式で必要なデータにアクセスする方法を提供するAPIと実装(選択可能性と効率の両立)
    // ## points
    // - (Storage設計は)Key(走査の軸)を何にするかが重要
    // - Systemでは対象Componentに一括で処理を行うことを基本とする(Entity毎ではない)
    // ## API Proposal
    // QUERY (world-independent):
    // auto q = query().select<Position>()
    //                .require<Velocity>()
    //                .exclude<Disabled>();
    // VIEW (world-dependent):
    // auto v = registry.view(q);
    // ## Flow
    // - Create a query
    // - Create a view by binding it to the Registry
    // - Execute the execution (behind the scenes)
    // Primitives
    // template <typename... Args>
    // struct All
    // {
    //     using Registry = ::ddknd::registry::Registry;
    //     using Entity = ::ddknd::entity::Entity;

    //     bool operator()(const Registry& reg, Entity e) const
    //     {
    //         return ((reg.TryGetComponent<Args>(e) != nullptr) && ...);
    //     }
    // };

    // template <typename... Args>
    // struct Any
    // {
    //     using Registry = ::ddknd::registry::Registry;
    //     using Entity = ::ddknd::entity::Entity;

    //     bool operator()(const Registry& reg, Entity e) const
    //     {
    //         if constexpr (sizeof...(Args) == 0)
    //             return true;
    //         return ((reg.TryGetComponent<Args>(e) != nullptr) || ...);
    //     }
    // };

    // // ======= Composer =======
    // template <typename L, typename R>
    // struct And
    // {
    //     using Registry = ::ddknd::registry::Registry;
    //     using Entity = ::ddknd::entity::Entity;

    //     L l;
    //     R r;
    //     bool operator()(const Registry& reg, Entity e) const
    //     {
    //         return l(reg, e) && r(reg, e);
    //     }
    // };

    // template <typename L, typename R>
    // struct Or
    // {
    //     using Registry = ::ddknd::registry::Registry;
    //     using Entity = ::ddknd::entity::Entity;

    //     L l;
    //     R r;
    //     bool operator()(const Registry& reg, Entity e) const
    //     {
    //         return l(reg, e) || r(reg, e);
    //     }
    // };

    // template <typename P>
    // struct Not
    // {
    //     P p;

    //     using Registry = ::ddknd::registry::Registry;
    //     using Entity = ::ddknd::entity::Entity;
    //     bool operator()(const Registry& reg, Entity e) const
    //     {
    //         return !p(reg, e);
    //     }
    // };
} // namespace ddknd::query