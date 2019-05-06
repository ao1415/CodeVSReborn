#pragma once

namespace Config {


	/// <summary>探査ターン数</summary>
	constexpr int Turn = 15;
	/// <summary>探査時のビーム幅</summary>
	constexpr int Chokudai = 3;
	/// <summary>探査時間</summary>
	constexpr int ThinkTime = 1500;

	/// <summary>許容ロスチェイン数</summary>
	constexpr int UselessChain = 1;
	/// <summary>お邪魔チェイン数</summary>
	constexpr int JammingChain = 4;
	/// <summary>妨害できる相手のチェイン数</summary>
	constexpr int ApprovalJammingChain = 5;
	/// <summary>お邪魔ゲージ数</summary>
	constexpr int JammingGauge = 60;
	/// <summary>お邪魔数</summary>
	constexpr int JammingGarbage = 80;

	/// <summary>発火チェイン数</summary>
	constexpr int ChainIgnition = 12;
	/// <summary>発火スキルスコア</summary>
	constexpr int SkillIgnitionScore = 160;

	/// <summary>許容お邪魔数</summary>
	constexpr int ToleranceGarbage = 30;
	/// <summary>許容お邪魔数</summary>
	constexpr int ToleranceCounterGarbage = 20;
	/// <summary>許容お邪魔数</summary>
	constexpr int LethalGarbage = 50;

	/// <summary>敵の探査ターン数</summary>
	constexpr int EnemyThinkTurn = 3;

	/// <summary>評価用ナンバーの取得ターン数</summary>
	constexpr int PackNumberTurn = 12;

}
