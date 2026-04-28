#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <type_traits>


namespace ddknd::core
{
    template <typename Tag, typename Rep = std::uint32_t>
    class StrongID
    {
      public:
        using rep_type = Rep;

        static_assert(std::is_integral_v<rep_type>, "StrongID<..., Rep>: Rep must be an integral type.");
        static_assert(std::is_unsigned_v<rep_type>, "StrongID<..., Rep>: Rep must be an unsigned integral type.");

        // return invalid value
        static constexpr StrongID Invalid() noexcept
        {
            return StrongID{invalid_value()};
        }

        constexpr StrongID() noexcept : value_(invalid_value()) {}
        explicit constexpr StrongID(rep_type v) noexcept : value_(v) {}

        // return raw value
        constexpr rep_type Value() const noexcept
        {
            return value_;
        }

        // Check if the value is invalid
        constexpr bool Is_valid() const noexcept
        {
            return value_ != invalid_value();
        }

        friend constexpr bool operator==(StrongID a, StrongID b) noexcept
        {
            return a.value_ == b.value_;
        }
        friend constexpr bool operator!=(StrongID a, StrongID b) noexcept
        {
            return !(a == b);
        }
        friend constexpr bool operator<(StrongID a, StrongID b) noexcept
        {
            return a.value_ < b.value_;
        }

      private:
        static constexpr rep_type invalid_value()
        {
            return std::numeric_limits<rep_type>::max();
        }

        rep_type value_;
    };

    // uint64_t: 32bit: gen / 32bit: id
    template <typename Tag>
    class HandleID
    {
      public:
        using rep_type = std::uint64_t;
        using tag_type = Tag;
        using strong_type = StrongID<Tag, rep_type>;

        constexpr HandleID() noexcept : id_(strong_type::Invalid()) {}

        constexpr HandleID(std::uint32_t gen, std::uint32_t index) noexcept : id_(strong_type(pack(gen, index))) {}

        // API
        constexpr rep_type Value() const noexcept
        {
            return id_.Value();
        }
        constexpr bool Is_valid() const noexcept
        {
            return id_.Is_valid();
        }

        constexpr std::uint32_t Index() const noexcept
        {
            return static_cast<std::uint32_t>(id_.Value() & 0xFFFFFFFFull);
        }
        constexpr std::uint32_t Generation() const noexcept
        {
            return static_cast<std::uint32_t>(id_.Value() >> 32);
        }
        friend constexpr bool operator==(HandleID a, HandleID b) noexcept
        {
            return a.id_ == b.id_;
        }
        friend constexpr bool operator!=(HandleID a, HandleID b) noexcept
        {
            return !(a == b);
        }
        friend constexpr bool operator<(HandleID a, HandleID b) noexcept
        {
            return a.id_ < b.id_;
        }

      private:
        static constexpr rep_type pack(std::uint32_t gen, std::uint32_t index) noexcept
        {
            return (static_cast<rep_type>(gen) << 32) | static_cast<rep_type>(index);
        }

        explicit constexpr HandleID(strong_type s) noexcept : id_(s) {}

        strong_type id_;
    };

    // specialized std::hash function
    template <typename ID>
    struct StrongIDHasher
    {
        std::size_t operator()(const ID& id) const noexcept
        {
            return std::hash<typename ID::rep_type>{}(id.Value());
        }
    };
} // namespace ddknd::core
