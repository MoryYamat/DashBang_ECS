#pragma once 

#include "Engine/ECS/Entity.h"

#include "Common/EngineNamespaceDecl.h"

#include <vector>

namespace Game::Combat::Skill::Context
{
	// スキル実行状況を保持するコンポーネント
	struct SkillExecutionContextComponent
	{
		eNsECS::Entity caster = eNsECS::Entity::INVALID; // スキルを発動したキャスター
		eNsECS::Entity skillInstance = eNsECS::Entity::INVALID; // SkillInstanceのEntity参照
		eNsECS::Entity spawnedHitArea = eNsECS::Entity::INVALID; // スキルのヒットエリア(攻撃範囲)のEntity
		//std::vector<eNsECS::Entity> spawnedHitAreas; // スキルのヒットエリア(攻撃範囲)のEntityリスト

		bool cancelOnInterrupt = false; // スタン・死亡などでスキルを中断するべきか
		bool cancelOnSkillCancel = false; // キャラクターのキャンセルでスキルをを中断するべきか

		bool isMarkedForCancel = false; // システム側からのキャンセル要求フラグ
		bool hasForceEnded = false; // 完全に終了したかどうかのフラグ

		// optional:最後のフェーズなどに対応するためのタイマー
		float timeSinceStart = 0.0f; 


	};
}