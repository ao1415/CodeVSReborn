#pragma once

namespace Config {


	/// <summary>�T���^�[����</summary>
	constexpr int Turn = 15;
	/// <summary>�T�����̃r�[����</summary>
	constexpr int Chokudai = 3;
	/// <summary>�T������</summary>
	constexpr int ThinkTime = 1500;

	/// <summary>���e���X�`�F�C����</summary>
	constexpr int UselessChain = 1;
	/// <summary>���ז��`�F�C����</summary>
	constexpr int JammingChain = 4;
	/// <summary>�W�Q�ł��鑊��̃`�F�C����</summary>
	constexpr int ApprovalJammingChain = 5;
	/// <summary>���ז��Q�[�W��</summary>
	constexpr int JammingGauge = 60;
	/// <summary>���ז���</summary>
	constexpr int JammingGarbage = 80;

	/// <summary>���΃`�F�C����</summary>
	constexpr int ChainIgnition = 12;
	/// <summary>���΃X�L���X�R�A</summary>
	constexpr int SkillIgnitionScore = 160;

	/// <summary>���e���ז���</summary>
	constexpr int ToleranceGarbage = 30;
	/// <summary>���e���ז���</summary>
	constexpr int ToleranceCounterGarbage = 20;
	/// <summary>���e���ז���</summary>
	constexpr int LethalGarbage = 50;

	/// <summary>�G�̒T���^�[����</summary>
	constexpr int EnemyThinkTurn = 3;

	/// <summary>�]���p�i���o�[�̎擾�^�[����</summary>
	constexpr int PackNumberTurn = 12;

}
