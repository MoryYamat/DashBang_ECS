// 論理識別子としての静的データ
// Static Data as Logical Identifiers

#pragma once

namespace SkillSystem 
{
	// SkillSlotの種類
	enum class SkillSlot
	{
		None = -1,

		Primary,// 基本攻撃
		Secondary,// 補助攻撃
		
		Ultimate,//
		Utility1,//
		Utility2,// 

		Count
	};
}
