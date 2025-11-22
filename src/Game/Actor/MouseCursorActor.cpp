#include "MouseCursorActor.h"

#include "Engine/ECS/Entity.h"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"

// 現在未使用
#include "Engine/ECS/Component/Input/MouseCursorComponent.h"

// 最新版
#include "Engine/ECS/Component/Input/AnalogInputComponent.h"

#include "Engine/ECS/Component/Input/InputBindingComponent.h"

#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"

#include "Engine/ECS/Ops/CoreOps.hpp"

// 
Game::Actor::AnalogInput::MouseCursorActor::MouseCursorActor(Engine::ECS::EntityMgr& ecs)
{
	//namespace Ops = Engine::ECS::Ops;
	//namespace Comp = Engine::ECS::Component;

	//Engine::ECS::Entity e = ecs.createEntity();

	//// cursor comp (廃止予定)
	//Comp::Input::MouseCursorComponent mouseComp;
	//Ops::Add<Comp::Input::MouseCursorComponent>(ecs, e, mouseComp);

	//// analog input (最新版 (25/06/01))
	//Ops::Add<Comp::Input::AnalogInputComponent>(ecs, e, Comp::Input::AnalogInputComponent{});

	//// logic 2d transform comp
	//Comp::Logic2D::Logic2DTransformComponent logic2DComp;
	//Ops::Add<Comp::Logic2D::Logic2DTransformComponent>(ecs, e, logic2DComp);

	//// player cotrolloer comp
	// Comp::Tags::PlayerControllerComponent playerComp;
	// Ops::Add<Comp::Tags::PlayerControllerComponent>(ecs, e, playerComp);

	//// binding
	// Ops::Add<Comp::Input::InputBindingComponent>(ecs, e, Comp::Input::InputBindingComponent{});
}