// Game Œn‚Ì–¼‘O‹óŠÔ‚Ì‘O•ûéŒ¾{alias’è‹`‚Ì‚İ‚É“Á‰»‚µ‚½ƒwƒb

#pragma once

// Game Namespace (gNs)
namespace Game
{
	namespace Feature 
	{
		namespace Combat {}
	}

	namespace Layer {}

	// actor initialize
	namespace Actor
	{
		namespace Map {}
		namespace Camera {}
		namespace AnalogInput {}
		namespace Character {}
		namespace Player {}
	}

	// character
	namespace Character
	{

		namespace Intent {}
		namespace Movement {}
		namespace State {}
		namespace Stats {}
	}

	// input
	namespace Input
	{
		namespace Intent {}

		namespace Analog {}

		// ”p~—\’è(Intent‚ğ‰î‚µ‚½‘€ìƒƒWƒbƒN‚ÉØ‚è‘Ö‚¦‚é‚½‚ß)
		// ”p~—\’è(Intent‚ğ‰î‚µ‚½‘€ìƒƒWƒbƒN‚ÉØ‚è‘Ö‚¦‚é‚½‚ß)
		// ”p~—\’è(Intent‚ğ‰î‚µ‚½‘€ìƒƒWƒbƒN‚ÉØ‚è‘Ö‚¦‚é‚½‚ß)
		namespace Player {}
	}

	// camera
	namespace Camera
	{
		namespace Input {}
	}

	namespace Collision
	{
		namespace Data {}
		namespace System {}
		namespace Extract {}
		namespace Utils {}
	}

	namespace Combat
	{
		namespace Skill
		{
			namespace Component {}
			namespace System {}
			namespace Factory {}
			namespace Trigger {}
			namespace Data {}
			namespace Utils {}

			namespace Intent {}
		}
	}

	namespace Init
	{
		namespace Logic2D {}
	}

	namespace Utils 
	{
		
	}

	namespace Test
	{
		namespace Actor {}
	}
}

// ======================= Alias =======================

// Game::Feature
namespace gNsFeature = Game::Feature;

// Game::Layer
namespace gNsLayer = Game::Layer;

// Game::Actor::
namespace gNsActor = Game::Actor;
namespace gNsActorCharacter = Game::Actor::Character;
namespace gNsActorMap = Game::Actor::Map;
namespace gNsActorAnalogInput = Game::Actor::AnalogInput;
namespace gNsActorCam = Game::Actor::Camera;
namespace gNsActorPlayer = Game::Actor::Player;

// Game::Character:: 
namespace gNsCharacter = Game::Character;
// 
namespace gNsCharacterIntent = Game::Character::Intent;// Character intent
namespace gNsCharacterMovement = Game::Character::Movement;// 
namespace gNsCharacterState = Game::Character::State;// Character state
namespace gNsCharacterStats = Game::Character::Stats;


// Game::Input:: 
namespace gNsInput = Game::Input;
namespace gNsInputIntent = Game::Input::Intent;// Player intention

// Game::Camera::
namespace gNsCam = Game::Camera;
namespace gNsCamInput = Game::Camera::Input;

// Game::Init::
namespace gNsInit = Game::Init;


// Game::Collision::
namespace gNsColl = Game::Collision;
namespace gNsCollData = Game::Collision::Data;// 
namespace gNsCollSystem = Game::Collision::System;
namespace gNsCollExtract = Game::Collision::Extract;
namespace gNsCollUtil = Game::Collision::Utils;

// Game::Combat::
namespace gNsSkillComp = Game::Combat::Skill::Component;
namespace gNsSkillFactory = Game::Combat::Skill::Factory;
namespace gNsSkillSystem = Game::Combat::Skill::System;
namespace gNsSkillData = Game::Combat::Skill::Data;
namespace gNsSkillTrigger = Game::Combat::Skill::Trigger;
namespace gNsSkillUtils = Game::Combat::Skill::Utils;
// Intent
namespace gNsSkillIntent = Game::Combat::Skill::Intent;