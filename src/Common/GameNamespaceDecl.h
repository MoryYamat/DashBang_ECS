// Game ånÇÃñºëOãÛä‘ÇÃëOï˚êÈåæÅ{aliasíËã`ÇÃÇ›Ç…ì¡âªÇµÇΩÉwÉb

#pragma once

// Game Namespace (gNs)
namespace Game
{
	namespace Common
	{
		namespace Logic
		{
			namespace FSM
			{
				namespace Runtime {}
				namespace Definition {}
			}

			namespace Condition {}
		}
	}

	namespace Feature 
	{
		namespace Character {}
		namespace Combat {}
		namespace Setting {}

		namespace ECS {}
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

	namespace ECS
	{
		namespace Component
		{

		}

		namespace Tags {}

		namespace System {}
	}

	// character
	namespace Character
	{

		namespace Intent {}
		namespace Control 
		{
			namespace Skill {}
		}
		namespace Movement {}
		namespace State 
		{
			namespace Life {}
			namespace Movement {}
			namespace Action {}
		}
		namespace Stats {}
	}

	// input
	namespace Input
	{
		namespace Intent {}

		namespace Analog {}

		// îpé~ó\íË(IntentÇâÓÇµÇΩëÄçÏÉçÉWÉbÉNÇ…êÿÇËë÷Ç¶ÇÈÇΩÇﬂ)
		// îpé~ó\íË(IntentÇâÓÇµÇΩëÄçÏÉçÉWÉbÉNÇ…êÿÇËë÷Ç¶ÇÈÇΩÇﬂ)
		// îpé~ó\íË(IntentÇâÓÇµÇΩëÄçÏÉçÉWÉbÉNÇ…êÿÇËë÷Ç¶ÇÈÇΩÇﬂ)
		namespace Player {}
	}

	// camera
	namespace Camera
	{
		namespace Input {}
	}

	namespace Collision
	{
		namespace Init {}
		namespace Component {}
		namespace Data {}
		namespace System {}
		namespace Extract {}
		namespace Utils {}
		namespace Convert {}
		namespace Intersect {}
	}

	namespace Combat
	{
		namespace Skill
		{
			namespace Component {}
			namespace System 
			{
				namespace Trigger {}
			}
			namespace Factory {}
			namespace Trigger {}
			namespace Data {}
			namespace MasterData {}
			namespace Utils {}

			namespace Intent {}

			namespace Context {}
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

// Game::Common
namespace gNsCommon = Game::Common;
namespace gNsCommonLogic = Game::Common::Logic;
namespace gNsCommonLogicFSM = Game::Common::Logic::FSM;
namespace gNsCommonLogicFSMRuntime = Game::Common::Logic::FSM::Runtime;
namespace gNsCommonLogicFSMDefinition = Game::Common::Logic::FSM::Definition;
namespace gNsCommonLogicCondition = Game::Common::Logic::Condition;

// Game::Feature
namespace gNsFeature = Game::Feature;

// Game::Layer
namespace gNsLayer = Game::Layer;

// ECS
namespace gNsTags = Game::ECS::Tags;
namespace gNsECSComp = Game::ECS::Component;
namespace gNsECSSystem = Game::ECS::System;

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
namespace gNsCharaLifeState = Game::Character::State::Life;
namespace gNsCharaMoveState = Game::Character::State::Movement;
namespace gNsCharaActionState = Game::Character::State::Action;
namespace gNsCharacterStats = Game::Character::Stats;
// Character control
namespace gNsCharacterControl = Game::Character::Control;// Character control
namespace gNsCharacterControlSkill = Game::Character::Control::Skill;// Character control skill

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
namespace gNsCollInit = Game::Collision::Init;
namespace gNsCollComp = Game::Collision::Component;
namespace gNsCollData = Game::Collision::Data;// 
namespace gNsCollSystem = Game::Collision::System;
namespace gNsCollExtract = Game::Collision::Extract;
namespace gNsCollUtil = Game::Collision::Utils;
namespace gNsCollConvert = Game::Collision::Convert;
namespace gNsCollIntersect = Game::Collision::Intersect;

// Game::Combat::
namespace gNsSkillComp = Game::Combat::Skill::Component;
namespace gNsSkillFactory = Game::Combat::Skill::Factory;
namespace gNsSkillSystem = Game::Combat::Skill::System;
namespace gNsSkillData = Game::Combat::Skill::Data;
namespace gNsSkillMasterData = Game::Combat::Skill::MasterData;
namespace gNsSkillTrigger = Game::Combat::Skill::Trigger;
namespace gNsSkillUtils = Game::Combat::Skill::Utils;
namespace gNsSkillContext = Game::Combat::Skill::Context;

// Intent
namespace gNsSkillIntent = Game::Combat::Skill::Intent;