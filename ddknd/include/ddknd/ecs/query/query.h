#pragma once

namespace ddknd::ecs
{
    template <typename... Ts>
    struct TypeList
    {
    };

    struct None
    {
    };

    // Describe component selection, requirements, and exclusions for an ECS view.
    // Each builder function is intended to be called onece and replaces its previous specification.
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

    inline auto query()
    {
        return Query<None, TypeList<>, TypeList<>>{};
    }
} // namespace ddknd::ecs