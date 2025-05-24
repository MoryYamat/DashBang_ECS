// skill trajectory data
#pragma once

#include <GLM/glm.hpp>

#include <variant>

enum class TrajectoryType
{
	None,
	RotateAroundSelf,// 
	LinearForward,// 
	CurverdSwing,// 
	// �C�ӂ̋O�Ճ^�C�v���g���\
};

namespace SkillTrajectory
{
	struct RotateTrajectoryParams
	{
		float startAngle;
		float endAngle;
	};

	struct LinearTrajectoryParams
	{
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
