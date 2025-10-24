#pragma once

#include "Engine/ECS/Entity.h"

#include <typeindex>
#include <optional>
#include <any>
#include <vector>

namespace Game::Character::FSM::Interference::Core::Data
{

	enum class InterferenceMode
	{
		None, 
		ForceTransition,// 特定の状態に強制遷移させる
		BlockInput,// 入力を無効化する
		Freeze,// 状態を固定
		// 必要に応じて拡張
	};

	enum class ControlSeverity : uint8_t
	{
		Low = 0,
		Medium,
		High,
		Critical
	};

	struct FSMInterferenceRequest
	{
		std::type_index issuerAxis;// 発行者FSM識別子
		std::type_index targetAxis;// 干渉対象のFSM

		ControlSeverity severity;


		InterferenceMode mode;
		std::optional<std::type_index> forcedState; // 強制遷移させたい状態

		float durationSec = 0.0f;// 最初干渉時間

		std::optional<Engine::ECS::Entity> issuerEntity; // Entity(optional) // 干渉の起点
		std::optional<Engine::ECS::Entity> targetEntity; // 対象のエンティティ

		FSMInterferenceRequest(std::type_index issuerAxis, std::type_index targetAxis) 
			: issuerAxis(issuerAxis), targetAxis(targetAxis), severity(ControlSeverity::Low), mode(InterferenceMode::None){}

		//std::any customContext; // 拡張情報・補足コンテキスト
	};

	// 雛形
	//struct FSMInterferenceRequestComponent
	//{
	//	std::vector<FSMInterferenceRequest> requests;

	//	bool hasActiveRequest()
	//	{
	//		for (const auto& req : requests)
	//		{
	//			if (req.durationSec > 0.0f)
	//			{
	//				return true;
	//			}
	//		}
	//		return false;
	//	}
	//};
}