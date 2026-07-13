#pragma once

#include <cstdint>

#include "ddknd/core/StrongID.h"

namespace ddknd::ecs
{
    struct Entity
    {
      private:
        struct Entity_ID
        {
        };
        using id_type = ::ddknd::core::HandleID<Entity_ID>;

      public:
        constexpr Entity() noexcept = default;
        explicit constexpr Entity(std::uint32_t gen, std::uint32_t index) noexcept : id(gen, index) {}

        static constexpr Entity Invalid() noexcept
        {
            return Entity{};
        }
        constexpr id_type::rep_type Value() const noexcept
        {
            return id.Value();
        }
        constexpr std::uint32_t Index() const noexcept
        {
            return id.Index();
        }
        constexpr std::uint32_t Gen() const noexcept
        {
            return id.Generation();
        }

        constexpr bool IsValid() const
        {
            return id.Is_valid();
        }

        // ops
        friend constexpr bool operator==(Entity a, Entity b)
        {
            return a.id == b.id;
        }
        friend constexpr bool operator!=(Entity a, Entity b)
        {
            return !(a == b);
        }
        friend constexpr bool operator<(Entity a, Entity b)
        {
            return a.id < b.id;
        }

      private:
        id_type id{};
    };
} // namespace ddknd::ecs