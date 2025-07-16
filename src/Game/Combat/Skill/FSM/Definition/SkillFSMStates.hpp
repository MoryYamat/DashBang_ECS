#pragma once



namespace Game::Combat::Skill::FSM
{
	struct SkillFSMAxis {};

	namespace SkillPhase
	{
		struct None {};
		struct Casting {};
		struct Active {};
		struct Recovery {};
		struct Completed {};
		struct Interrupted {};
	}


}

