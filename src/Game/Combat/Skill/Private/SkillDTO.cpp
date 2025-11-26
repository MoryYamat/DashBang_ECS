#include "Game/Combat/Skill/Public/DTO.hpp"

#include "Game/Combat/Skill/Public/Builder.hpp"

namespace Game::Combat::Skill
{
	static void make_skillDTO(SkillCatalogBuilder& builder)
	{
		SkillDefDTO def;

		def.fsm = "Basic";
		def.name = "skill_test";
		def.profile = "Default";
		def.phases = { 
			SkillPhaseDTO{.name = "Casting", .duration = 1.0f}, 
			SkillPhaseDTO{.name = "Acitve", .duration = 1.0f}, 
			SkillPhaseDTO{.name = "Recovery", .duration = 1.0f} };
		def.effects = { SkillEffectDefDTO{.phase = "Active", .timeOffset = 0.2f, .kind = "SpawnHitbox"} };

		builder.add(std::move(def));
	}

	void RegisterSkillDef(SkillCatalogBuilder& builder)
	{
		make_skillDTO(builder);
	}
}