// 作者が定義

#pragma once

#include <string>
#include <vector>

namespace Game::Combat::Skill
{
	struct SkillPhaseDTO
	{
		std::string name;			// "Casting", "Active",... // FSM定義と同じにする必要あり
		float duration;				// フェーズの基準時間
		// 将来: cancelWindowStart, cancelWindowEndなど
	};


	struct SkillEffectDefDTO
	{
		std::string phase;			// どのフェーズで
		float timeOffset;			// フェーズ開始から何秒後に
		std::string kind;			// "SpawnHitbox", "PlayAnim",...
		//std::string param;			// ヒットボックスID や アニメ名など
	};

	struct SkillDefDTO
	{
		std::string name;			// skill名"Skill_211"
		std::vector<SkillPhaseDTO> phases;
		std::vector<SkillEffectDefDTO> effects;

		// FSM/Profile名への紐づけなど
		std::string fsm;			// (基本は"Basic")  fsm のどの遷移構造(幾何的グラフ)を使用するか
		std::string profile;		// (基本は"Default")skill fsm の どのプロファイルを使用するか (どのSkillFSMのfsm定義を使用するかという自由度もあるが今は1つしかFSM定義がないと仮定する)
	};

}