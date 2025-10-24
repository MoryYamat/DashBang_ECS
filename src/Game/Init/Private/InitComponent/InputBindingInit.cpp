#include "InputBindingInit.h"

#include "Engine/ECS/Component/Input/AnalogInputComponent.h"
#include "Engine/ECS/Component/Input/InputBindingComponent.h"

#include "Game/ECS/Private/Tags/CharacterAttribTags.h"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"


void Game::Init::Input::InputBindingInitializationSystem(Engine::ECS::EntityMgr& ecs)
{
	for (Engine::ECS::Entity cursor : ecs.view<
		Engine::ECS::Component::Input::AnalogInputComponent,
		Engine::ECS::Component::Input::InputBindingComponent>())
	{
		auto& binding = ecs.get<Engine::ECS::Component::Input::InputBindingComponent>(cursor);

		for (Engine::ECS::Entity Player : ecs.view<
			Game::ECS::Tags::PlayerCharacterTag>())
		{
			binding.controllingEntity = Player;

			std::cout << "[InputBindingInit(palyer-analoginput)] palyer: " << Player.id << " binding: " << binding.controllingEntity.id << std::endl;
		}
	}
}

namespace Game::Init::Input
{
	void InputBindingInitializationSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{

	}
}