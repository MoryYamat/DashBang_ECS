#include "MouseCursorActor.h"

#include "Core/ECS/Entity.h"

#include "Core/ECS/Component/Logic2DTransformComponent.h"
#include "Core/ECS/Component/MouseCursorComponent.h"

#include "Core/ECS/Component/Tags/PlayerControllerComponent.h"

// 
MouseCursorActor::MouseCursorActor(ECS& ecs)
{
	Entity entity = ecs.createEntity();

	// cursor comp
	MouseCursorComponent mouseComp;
	ecs.addComponent(entity, mouseComp);

	// logic 2d transform comp
	Logic2DTransformComponent logic2DComp;
	ecs.addComponent(entity, logic2DComp);

	// player cotrolloer comp
	PlayerControllerComponent playerComp;
	ecs.addComponent(entity, playerComp);
}