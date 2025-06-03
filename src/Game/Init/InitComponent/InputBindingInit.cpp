#include "InputBindingInit.h"

#include "Engine/ECS/Component/Input/AnalogInputComponent.h"
#include "Engine/ECS/Component/Input/InputBindingComponent.h"

#include "Engine/ECS/Component/Tags/PlayerCharacterTag.h"

void Game::Init::Input::InputBindingInitializationSystem(eNsECS::EntityMgr& ecs)
{
	for (eNsECS::Entity cursor : ecs.view<
		eNsInputComp::AnalogInputComponent,
		eNsInputComp::InputBindingComponent>())
	{
		auto& binding = ecs.get<eNsInputComp::InputBindingComponent>(cursor);

		for (eNsECS::Entity Player : ecs.view<
			eNsTagComp::PlayerCharacterTag>())
		{
			binding.controllingEntity = Player;

			std::cout << "[InputBindingInit(palyer-analoginput)] palyer: " << Player.id << " binding: " << binding.controllingEntity.id << std::endl;
		}
	}
}