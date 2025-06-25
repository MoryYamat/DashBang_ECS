// Mapping InputAction and SkillId
// InputActionとSkillIdのマッピング

#pragma once

#include <unordered_map>
#include "Game/Input/InputAction.h"

#include "Game/Combat/Skill/MasterData/SkillSlot.h"

#include "Common/EngineNamespaceDecl.h"
#include "Common/GameNamespaceDecl.h"

// 現在未使用のため削除予定
namespace Game::Combat::Skill::Trigger
{
	//class SkillInputMap
	//{
	//public:
	//	void bind(gNsInput::InputAction action, gNsSkillData::SkillSlot slot)
	//	{
	//		mapping[action] = slot;
	//	}

	//	bool hasBinding(gNsInput::InputAction action) const
	//	{
	//		return mapping.find(action) != mapping.end();
	//	}

	//	gNsSkillData::SkillSlot getSkillID(gNsInput::InputAction action) const
	//	{
	//		auto it = mapping.find(action);
	//		if (it != mapping.end())
	//			return it->second;
	//		else
	//			return gNsSkillData::SkillSlot::None;// 無効ID
	//	}

	//	const std::unordered_map<gNsInput::InputAction, gNsSkillData::SkillSlot>& getAllBindings() const
	//	{
	//		return mapping;
	//	}

	//private:
	//	std::unordered_map<gNsInput::InputAction, gNsSkillData::SkillSlot> mapping;
	//};
}

