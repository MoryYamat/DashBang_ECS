// Data Transfer Object

#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <utility>

#include <unordered_map>

namespace Engine::FSM::Base
{
	struct AxisDTO
	{
		std::string axis;						// "Skill"
		std::vector<std::string> stateU;		// 軸全域の状態名
		std::vector<std::string> condU;			// 軸全域の条件名
		std::vector<std::string> slotU;			// 軸全域のスロット名
		std::vector<std::string> profileU;		// 軸全域のプロファイル名
		std::uint16_t version = 1;
	};

	struct TransitionDTO
	{
		std::string from;
		std::string to;
		std::string slot;
		std::uint8_t prio = 0;
	};

	struct ProfileBindDTO
	{
		std::string slot;
		std::string cond;
	};

	struct ProfileDefDTO
	{
		std::string name;
		std::string extends;
		std::vector<ProfileBindDTO> binds;
	};

	struct FSMDTO
	{
		std::string axis;
		std::string fsm;
		std::vector<std::string> profiles;
		std::vector<std::string> states;
		std::vector<ProfileDefDTO> profile_defs;
		std::vector<TransitionDTO> transitions;
		std::uint16_t version = 1;
	};




	//struct AuthoringStateSet
	//{
	//	std::string SS_name;// State Set name
	//	std::vector<std::string> SS; // state (from SU)
	//};

	//struct AuthoringConditionSlot
	//{
	//	std::string CS_name;				// Slot name (e.g., "PhaseAdvance")
	//	std::vector<std::string> allowedCU; // allowed condition *labels* from CU (STCS)
	//};

	//struct AuthoringTransitionSlot
	//{
	//	std::string SS_name;// 属する状態集合(state set 名) 
	//	std::string from;// state name from SU
	//	std::string to; // state name from SU
	//	std::string slot;// CS_name
	//	std::uint8_t basePrio = 0;// 基礎優先度
	//};

	//// 条件セット全体の中身を差し替えるためのprofile
	//struct AuthoringConditionProfile
	//{
	//	std::string profile_name;

	//	// Slotごとに「採用するCUラベル」を1つ束縛
	//	std::vector<std::pair<std::string /*slot*/, std::string /*CU_label*/>> bindings;
	//};

	//struct AuthoringFSMSet
	//{
	//	std::string FSMSubset_name;
	//	std::vector<AuthoringStateSet> SS;			// state sets
	//	std::vector<AuthoringConditionSlot> CS;		// slot -> allowed CU labels
	//	std::vector<AuthoringTransitionSlot> TS;	// (from, to, slot) の列
	//	std::vector<AuthoringConditionProfile> PF;	// Profile(差し替え束縛)
	//};

	//struct AuthoringFSMUniverse
	//{
	//	std::string axis;
	//	std::vector<std::string> SU;		// State Universe
	//	std::vector<std::string> CU;		// Condition Universe (選択可能な「条件式ラベル」)
	//	std::vector<AuthoringFSMSet> FS;	// fsm sets;
	//	std::uint32_t version = 1;
	//};


}

// Use Case (json)
// {
//	"axis": "Skill",
//	"version": 1,
//	"stateU": ["Casting", "Active", "Recovery"],
//	"condU": ["TimeElapsed", "AnimHit", "HitCountGte"],
//	"slotU": ["PhaseAdvance", "Cancel"],
//	"profiles": {
//		"Default": {"PhaseAdvance": "TimeElapsed", "Cancel": "HitCoutGte"},
//		"Skill_221": {"extends": "Default", "PhaseAdvance": "AnimHit"}
//	},
//	"slotAllowed": {
//		"PhaseAdvance": ["AnimHit", "TimeElapsed"],
//		"Cancel": ["HitCountGte"]
//  },
//	"slotPolicy": { "PhaseAdvance": "open", "Cancel": "closed"},
//	"fsms": [{
//		"name": "Skill_Phase",
//		"states": ["Casting", "Active", "Recovery"],
//		"transitions": [
//			{"from": "Casting", "to": "Active", "slot": "PhaseAdvance", "prio": 0},
//			{"from": "Active", "to": "Recovery", "slot": "PhaseAdvance", "prio": 0}
//		]
//	}]
// }
//

// Use Case (yaml)
// 軸マニフェスト
// 
// axis: Skill
// stateU: [ Cating, Active, Recovery]
// condU: [TimeElapsed, AnimHit, HitCountGte]
// SlotU: [PhaseAdvance, Cancel]
// ProfileU: [Default] 
// version: 1
// -------------------------------------
//
// FSM定義
// fsm: Skill_Phase
// 
// profiles: [Default, Skill_211]
// 
// # トポロジ (構造)
// states: [Casting, Active, Recovery]
// 
// # Profile"束"の宣言(slot->cond)
// profile_defs:
//	Default:
//		PhaseAdvance: TimeElapsed
//		Cancel:		  HitCountGte
//  Skill_221:
//		extends: Default
//		PhaseAdvance: AnimHit
// 
// 
// # 遷移 (構造 + slot)
// transitions:
// - {from: Casting, to: Active, slot: PhaseAdvance, prio: 0}
// - {from: Active, to: Recovery, slot: PhaseAdvance, prio: 0}
//