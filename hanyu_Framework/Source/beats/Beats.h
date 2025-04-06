//CSVから曲の情報を取得して、譜面を作るクラス
#pragma once
#include "Fwk/Graphics.h"
#include "Lib/Math/Vector.h"
using namespace Lib::Math;

#include "Fwk/File/CSVFile.h"
#include "Fwk/Audio/SoundSource.h"
#include "Fwk/Audio/MP3Sound.h"


// publicの関数
// 初期化・更新・描画・解放
// タイマー・時間の長さを渡す・再生されているか

class Beats
{
public:
	void Init();
	void Update();
	void Render();
	void Term();
	int GetBeatsCounts();
	float GetTimer();
	float GetSongLength();
	float GetBeats();
	bool IsPlaying();
	void CreateMusic(CSVData* pCsvData);
	void StopBeats();


private:
	string musicName;
	string musicFileName;
	float mSongLength;
	float mTimer;
	float mBeatTimer;
	float mBeats;
	int mBPM;
	int mBeatsCounts;
	int mBeatsLength;
	SoundSource mSoundSource;
	MP3Sound mSound;

};