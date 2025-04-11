#pragma once

//ポーズ
#include "Scene/Pause.h"

#include <vector>
using namespace std;

class PauseDataMst
{
public:
	//初期化
	void Init();
	//終了
	void Term();
	//データを返す
	int Size();
	//指定した番号のデータを返す
	PauseData* Get(int index);

private:
	//ポーズ情報
	vector<PauseData> mData;
};

//ポーズ情報マスタを取得する
PauseDataMst& GetPauseDataMst();