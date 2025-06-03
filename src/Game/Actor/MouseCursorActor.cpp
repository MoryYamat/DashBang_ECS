#include "MouseCursorActor.h"

#include "Engine/ECS/Entity.h"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"

// åªç›ñ¢égóp
#include "Engine/ECS/Component/Input/MouseCursorComponent.h"

// ç≈êVî≈
#include "Engine/ECS/Component/Input/AnalogInputComponent.h"

#include "Engine/ECS/Component/Input/InputBindingComponent.h"

#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"

// 
Game::Actor::AnalogInput::MouseCursorActor::MouseCursorActor(eNsECS::EntityMgr& ecs)
{
	eNsECS::Entity entity = ecs.createEntity();

	// cursor comp (îpé~ó\íË)
	eNsInputComp::MouseCursorComponent mouseComp;
	ecs.addComponent(entity, mouseComp);

	// analog input (ç≈êVî≈ (25/06/01))
	ecs.addComponent(entity, eNsInputComp::AnalogInputComponent{});

	// logic 2d transform comp
	eNsLogic2DComp::Logic2DTransformComponent logic2DComp;
	ecs.addComponent(entity, logic2DComp);

	// player cotrolloer comp
	 eNsTagComp::PlayerControllerComponent playerComp;
	 ecs.addComponent(entity, playerComp);

	// binding
	ecs.addComponent(entity, eNsInputComp::InputBindingComponent{});
}