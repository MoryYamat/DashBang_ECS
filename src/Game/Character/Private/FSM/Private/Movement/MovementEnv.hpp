#pragma once
//
//#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/Core/Types.hpp"

#include <cstdint>
#include <vector>

namespace Game::Character::FSM::Movement
{
	struct MovementEnv final : Engine::FSM::Core::EnvSnapshot
	{
		bool canMove = true;// CCなどによって移動不可能な場合はfalse
		bool hasMoveInput = false;// 入力意図により移動しようとしているかどうか

		std::vector<std::uint32_t> flags;// CondID.vに該当するビット位置に「現在の条件が真かどうか」を格納する

		// ビットが立っているかを判定
		bool testCondBit(std::uint32_t i) const override
		{
			const std::uint32_t w = i >> 5;// i / 32 // flagsが属するwordブロック
			const std::uint32_t b = i & 31;// i % 32 // 何番目のビットか
			return (w < flags.size()) && (flags[w] & (1ull << b));
		}

		// 条件番号i番目のビットをON/OFFする関数
		void set(std::uint32_t i, bool v)
		{
			const std::uint32_t w = i >> 5;
			const std::uint32_t b = i & 31;

			// 追加
			if (w >= flags.size()) flags.resize(w + 1, 0);

			const std::uint32_t m = 1ull << b;
			// 
			if (v) flags[w] |= m;
			else flags[w] &= ~m;
		}
	};
}

//
////Game/Character/Private/FSM/Movement/
////├─ MovementEnv.hpp
////├─ MovementEnvBuilder.hpp
////├─ MovementEnvBuilder.cpp
////├─ MovementCondFactory.hpp
////├─ MovementCondFactory.cpp
////└─ MovementSelectorSystem.cpp   // 実行サンプル（更新ループ側）
