#include "Game/Character/Private/Input/Public/InputApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Engine/Component/Private/Input/AnalogInputComponent.hpp"
#include "Engine/InputManager/Private/RawInputState.h"

#include "Game/Character/Private/Input/Private/InputActionComponent.hpp"// FIXME: 定義場所変更を検討する必要あり
#include "Game/Character/Private/Input/Private/InputMapping.hpp"		// FIXME: 定義場所変更を検討する必要あり

#include "Game/Utils/Private/SpatialTransformUtils.h"

namespace Game::Character::Input
{
	using namespace Engine::WorldSystem::Query;
	using namespace Engine::Component;

	// analog
	void AnalogInputRouting(Engine::WorldSystem::Core::WorldCtx& ctx
		, const Engine::Input::RawInputState& rawInput
		, const Engine::Graphics::Render::RenderContext& renderContext)
	{
		auto ent = ViewWhere(ctx.rw, All<AnalogInputComponent>{});

		for (auto e : ent)
		{
			auto& analog = ctx.ww.Get<AnalogInputComponent>(e);

			analog.cursorLogicPositionXZ = Game::Utils::ProjectScreenToLogicXZPlane(rawInput.mousePosition, renderContext);
			analog.cursorDelta = rawInput.mouseDelta;
			analog.scrollDelta = rawInput.scrollDelta;
		}
	}

	// key 
	void KeyInputRouting(Engine::WorldSystem::Core::WorldCtx& ctx,
		const Engine::Input::RawInputState& rawInput)
	{

		const auto& map = ctx.rw.GetResource<Game::Character::Input::RawToInputAction>();

		auto ent = ViewWhere(ctx.rw, All<Game::Character::Input::InputActionComponent>{});

		for (auto e : ent)
		{
			auto& input = ctx.ww.Get<Game::Character::Input::InputActionComponent>(e);

			input.previous = input.current;
			input.current.clear();

			for (const auto& [key, isDown] : rawInput.keyState)
			{
				if (!isDown) continue;

				if (auto actionOpt = map.getAction(key))
				{
					// 状態更新
					input.current[*actionOpt] = true;
				}
			}
		}
	}
}