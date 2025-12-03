#include "Game/Combat/Skill/Public/Core/DTO.hpp"

#include "Game/Combat/Skill/Private/Core/SkillCatalogBuilder.hpp"

namespace Game::Combat::Skill
{
	static void make_skillDTO(SkillCatalogBuilder& builder)
	{
		SkillDefDTO test_skill;

		test_skill.fsm = "Basic";
		test_skill.name = "test_skill";
		test_skill.profile = "Default";
		test_skill.phases = {
			SkillPhaseDTO{.name = "Casting", .duration = 1.0f}, 
			SkillPhaseDTO{.name = "Active", .duration = 1.0f}, 
			SkillPhaseDTO{.name = "Recovery", .duration = 1.0f} };
		test_skill.effects = {
			SkillEffectDefDTO{.phase = "Active", .timeOffset = 0.2f, .lifetime = 1.0f, .kind = "SpawnHitbox"},
			SkillEffectDefDTO{.phase = "Casting", .timeOffset = 0.1f, .kind = "PlayAnim"},
		};

		builder.add(std::move(test_skill));
	}

	void RegisterSkillDef(SkillCatalogBuilder& builder)
	{
		make_skillDTO(builder);
	}
}

namespace Game::Combat::Skill
{
	static void make_Slot2SkillDTO(SkillSlotLoadoutDTO& out)
	{
		out.slotSkillnames = { "test_skill" };
	}

}