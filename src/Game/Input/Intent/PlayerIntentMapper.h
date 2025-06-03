// 意味的入力(InputAction)をキャラクターの意図(Intent)に変換するシステム

#pragma once 

#include "Engine/ECS/EntityManager.h"

#include "Game/Input/InputActionComponent.h"
#include "Engine/ECS/Component/Input/AnalogInputComponent.h"
#include "Engine/ECS/Component/Input/InputBindingComponent.h"


#include "Game/Character/Movement/Component/Intent/MovementIntentComponent.h"
#include "Game/Character/Movement/Component/Intent/FacingIntentComponent.h"


#include "Engine/InputManager/RawInputState.h"
#include "Engine/Graphics/Renderer/RenderContext.h"

#include "Common/GameNamespaceDecl.h"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"

#include "Common/EngineNamespaceDecl.h"

#include "Common/GameNamespaceDecl.h"

namespace Game::Input::Intent
{

	struct IntentMappingSystem
	{
		// 入力状態をもとにIntentコンポーネントへ反映する
		static void UpdatePlayerIntent(eNsECS::EntityMgr& ecs
			, const eNsGfxRender::RenderContext& renderContext
		);

		// ワールド基準移動 (Global X-Z)
		static void updatePlayerMovementIntent(
			gNsCharacterIntent::MovementIntentComponent& intent
			, gNsInput::InputActionComponent& input
		);


		// mouse input -> intent facing
		static void updatePlayerDirectionIntent(
			eNsInputComp::AnalogInputComponent& analog,
			gNsCharacterIntent::FacingIntentComponent& facing,
			eNsLogic2DComp::Logic2DTransformComponent& logic
		);
	};
}