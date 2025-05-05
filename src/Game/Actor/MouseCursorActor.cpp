#include "MouseCursorActor.h"

#include "Core/ECS/Entity.h"

#include "Core/ECS/Component/Logic2DTransformComponent.h"
#include "Core/ECS/Component/MouseCursorComponent.h"

MouseCursorActor::MouseCursorActor(ECS& ecs)
{
	Entity entity = ecs.createEntity();

	MouseCursorComponent mouseComp;

	Logic2DTransformComponent logic2DComp;

	ecs.addComponent(entity, mouseComp);
	ecs.addComponent(entity, logic2DComp);
}