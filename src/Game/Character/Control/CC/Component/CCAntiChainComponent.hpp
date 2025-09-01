#pragma once


namespace Game::Character::Control::CC::Component
{
	/// <summary>
	/// 1. 各（キャラクタ）アクタのEntityに付与される
	/// 2. 動作
	/// 時間軸: ----|----|----|----|----|----|
	///				^windowStart
	///				<-------6 sec ------->
	/// </summary>
	struct CCAntiChainComponent
	{
		// 連鎖カウント
		int    count = 0;
		float windowStart = 0.0;
		float lastStrikeAt = -1.0;   // 同フレーム二重計上の防止に使える


		bool   immune = false;        // IMMUNE状態をUI等に見せたい場合に参照
		float immuneUntil = 0.0;     // ApplyImmune の期限（参考値）

		// ヘルパ（System側から使う）
		void resetWindow(float now) { count = 0; windowStart = now; }
		void clearImmune() { immune = false; immuneUntil = 0.0; }
	};
}