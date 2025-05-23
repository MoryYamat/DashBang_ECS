// 初期状態を基準とした差分や補正値
#pragma once

#include <GLM/glm.hpp>

#include <variant>

enum class TrajectoryType
{
	None,
	RotateAroundSelf,// 
	LinearForward,// 
	CurverdSwing,// 
	// 任意の軌跡タイプを拡張可能
};

namespace SkillTrajectory
{
	struct RotateTrajectoryParams
	{
		// degrees
		float startAngle = 0.0f;
		float endAngle = 0.0f;
	};

	struct LinearTrajectoryParams
	{
		// units per sec
		float speed;
	};

	struct CurvedTrajectoryParams
	{
		glm::vec2 controlPoint;
	};

	using TrajectoryParamsVariant = std::variant<std::monostate
		, RotateTrajectoryParams
		, LinearTrajectoryParams
		, CurvedTrajectoryParams>;

	template <typename VariantT>
	struct VariantHelper
	{
		template<typename T>
		static bool Is(const VariantT& v) { return std::holds_alternative<T>(v); }

		template<typename T>
		static T& As(VariantT& v)
		{
			return std::get<T>(v);
		}

		template<typename T>
		static const T& As(const VariantT& v)
		{
			return std::get<T>(v);
		}

		template<typename T>
		static T* TryAs(VariantT& v)
		{
			return std::holds_alternative<T>(v) ? &std::get<T>(v) : nullptr;
		}
	};
}
