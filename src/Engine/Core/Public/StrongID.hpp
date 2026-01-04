#include <cstddef>
#include <limits>
#include <type_traits>
#include <functional>

namespace Engine::Core
{
	template<typename T, typename Rep = std::uint32_t>
	class StrongID
	{
	public:
		using rep_type = Rep;

		static constexpr StrongID Invalid() noexcept
		{
			return StrongID{ invalid_value() };
		}

		constexpr StrongID() noexcept : value_(invalid_value()) {}
		explicit constexpr StrongID(Rep v) noexcept : value_(v) {}

		constexpr Rep value() const noexcept { return value_; }
		constexpr bool is_valid() const noexcept { return value_ != invalid_value(); }

		friend constexpr bool operator==(StrongID a, StrongID b) noexcept { return a.value == b.value; }
		friend constexpr bool operator!=(StrongID a, StrongID b) noexcept { return !(a == b); }
		friend constexpr bool operator<(StrongID a, StrongID b) noexcept { return a.value < b.value; }

	private:

		static consteval Rep invalid_value()
		{
			return std::numeric_limits<Rep>::max();
		}

		Rep value_;
	};
}

namespace std
{
	template<typename Tag, typename Rep>
	struct std::hash<Engine::Core::StrongID<Tag, Rep>>
	{
		size_t operator()(Engine::Core::StrongID<Tag, Rep> id) const noexcept
		{
			return std::hash<Rep>{}(id.value());
		}
	};
}