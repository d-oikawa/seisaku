#pragma once

//ゲームプレイ情報クラス
//ステージの番号・クリア済みフラグのアクセサ
class  GamePlayData
{
public:
	void Init();
	int GetStageNo();
	void SetStageNo(int stageNo);

	bool IsStageCleared(int stageNo);
	void SetStageState(int stageNo, bool isCleared);
private:
	int mStageNo;

	enum
	{
		CLEAR_FLAG_ELEMENT_NUM = 4
	};
	unsigned char mClearFlags[CLEAR_FLAG_ELEMENT_NUM];
};


//ゲームプレイ情報の取得
//GamePlayData& GetGamePlayData();
