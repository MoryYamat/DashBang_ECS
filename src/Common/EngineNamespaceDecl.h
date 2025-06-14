// Engineの名前空間の前方宣言+エイリアス

// ------------ Graphics -------------
// nsGfxRender = Engine::Graphics::Render;
// nsGfxModel = Engine::Graphics::Model;
// 
// ------------ Math -------------
// nsMathLogic = Engine::Math::Logic;
// nsMathRender = Engine::Math::Render;
// 
// ------------ ECS --------------
// nsECS = Engine::ECS;// EntityMgr
// nsUtilsComp = Engine::ECS::Component::Util;// Utils
// nsGfxComp = Engine::ECS::Component::Graphics;// Graphics
// nsInputComp = Engine::ECS::Component::Input; // Input
// nsTagComp = Engine::ECS::Component::Tags; // Tags
// nsCamComp = Engine::ECS::Component::Camera; // Camera
// nsLogic2DComp = Engine::ECS::Component::Logic2D; // Logic2D
// nsCommonComp = Engine::ECS::Component::Common; // Common
// 
// 
// ------------ Physics -------------
// namespace nsPhysColl = Engine::Physics::Collision; // Engine Collision
// 
// 
// 
// 

#pragma once

// Engine Namespace (eNs)
namespace Engine
{
	// Gfx
	namespace Graphics
	{
		namespace Render {}
		namespace Model {}
	}

	namespace Math
	{
		namespace Logic2D {}
		namespace Render {}
	}

	namespace ECS
	{
		namespace Detail {}

		namespace Component
		{
			namespace Utils {}// ECS utilities (e.g. name)

			namespace Tags {} // For General tagging components
			namespace Graphics {} // For render-related ECS data
			namespace Input {} // For input-related ECS data

			namespace Camera {} // For camera-related data
			
			namespace Logic2D {} // For 2D logic components
			
			namespace Common {} // Shared, generic components(e.g. Transform)
		}

		namespace Meta
		{
			namespace Init {}
		}

		namespace EntityUtils
		{

		}
	}

	namespace Input
	{

	}

	namespace Physics
	{
		namespace Logic2D
		{
			namespace Collision {}
		}

	}

	namespace Window
	{

	}

	namespace Sync
	{
		namespace LogicToTransformSystem {}
	}

	namespace Debug
	{
		namespace Logging {}
		namespace Drawing 
		{
			namespace Logic2D {}
		}
	}
}


// ======================= Alias =======================

// Engine::Graphics::
namespace eNsGfxRender = Engine::Graphics::Render;// 
namespace eNsGfxModel = Engine::Graphics::Model;

// Engine::Math
namespace eNsMath = Engine::Math;
namespace eNsLogic2DMath = Engine::Math::Logic2D;
namespace eNsRenderMath = Engine::Math::Render;

// Engine::ECS::Component
namespace eNsECS = Engine::ECS;// EntityMgr
namespace eNsUtilComp = Engine::ECS::Component::Utils;// Utils
namespace eNsGfxComp = Engine::ECS::Component::Graphics;// Graphics
namespace eNsInputComp = Engine::ECS::Component::Input; // Input
namespace eNsTagComp = Engine::ECS::Component::Tags; // Tags
namespace eNsCamComp = Engine::ECS::Component::Camera; // Camera
namespace eNsLogic2DComp = Engine::ECS::Component::Logic2D; // Logic2D
namespace eNsCommonComp = Engine::ECS::Component::Common; // Common

// Engine::ECS::Meta
namespace eNsECSInitComp = Engine::ECS::Meta::Init; // 

// Engine::Input
namespace eNsInput = Engine::Input;

// Engine::Phaysics
namespace eNsPhys2D = Engine::Physics::Logic2D; // Engine Collision
namespace eNsPhys2DColl = Engine::Physics::Logic2D::Collision;

// Engine::Window::
namespace eNsWindow = Engine::Window;

// Engine::Sync::
namespace eNsSync = Engine::Sync;
namespace eNsSyncL2T = Engine::Sync::LogicToTransformSystem;

// Engine::Debug
namespace eNsDebugLog = Engine::Debug::Logging;
namespace eNsDebugDraw = Engine::Debug::Drawing;