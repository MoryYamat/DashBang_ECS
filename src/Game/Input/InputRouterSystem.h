// RawInput + Mapping Å® ActionComponent Ç…ïœä∑	
// Convert RawInput + Mapping to ActionComponent

#pragma once

#include "Core/ECS/EntityManager.h"
#include "Core/InputManager/RawInputState.h"
#include "Game/Input/InputMapping.h"
#include "Core/ECS/Component/InputActionComponent.h"

void InputRouterSystem(ECS& ecs, const RawInputState& rawInput, const InputMapping& mapping);