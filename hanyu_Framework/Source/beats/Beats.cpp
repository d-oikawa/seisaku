#include "Beats.h"
#include <Fwk/Framework.h>

//初期化
//音楽・タイマーの初期化
//１拍あたりの秒数の計算 
//1小節の拍数×小節数＝総拍数
//総拍数×BPM分の60＝演奏時間
//演奏時間÷総拍数＝1拍当たりの秒数4拍子

void Beats::Init() {
		mSound.Load(musicFileName);
		mSoundSource.Init(mSound);
		mSoundSource.Play();
		mBeats = mSongLength / (float)(8 * 24);
}

//更新
//タイマー・拍の更新
void Beats::Update() {
	mTimer += Time_I->GetDeltaTime();
	mBeatTimer += Time_I->GetDeltaTime();
	if (mBeats<mBeatTimer)
	{
		mBeatsCounts++;
		mBeatTimer = 0.0f;
	}
}

//描画
//なし
void Beats::Render() {

}

//サウンドの開放
void Beats::Term() {
		mSoundSource.Stop();
		mSoundSource.Term();
		mSound.Unload();
}

//CSVから音楽データを取得する
//CSVの情報は、ファイル名、名前、曲全体の長さ(秒数)、BPM、何小節か
void Beats::CreateMusic(CSVData* pCsvData) {
	if (pCsvData == nullptr)
	{
		return;
	}

	int musicNum = pCsvData->GetElementCount() / 4;
	for (int i = 0; i < musicNum; i++)
	{
		int index = i * 4;
		pCsvData->GetString(index, &musicFileName);
		pCsvData->GetString(index + 1, &musicName);
		mSongLength = pCsvData->GetInt(index + 2);
		mBPM = pCsvData->GetInt(index + 3);
		mBeatsLength = pCsvData->GetInt(index + 4);
	}
	
}

//現在が曲全体で何拍目かを返す
int Beats::GetBeatsCounts() {
	return mBeatsCounts;
}
//現在の時間を返す
float Beats::GetTimer() {
	return mTimer;
}
//1拍の長さを返す
float Beats::GetBeats() {
	return mBeats;
}

//曲が再生されているか返す
//mSonglemgthよりmTimerの値が大きくなったらtrueを返す
bool Beats::IsPlaying() {
	bool bEnd = false;
	if (mSongLength<mTimer)
	{
		bEnd = true;
	}
	return bEnd;
}

//曲の長さを返す
float Beats::GetSongLength() {
	return mSongLength;
}

//曲を止める
void Beats::StopBeats() {
	mSoundSource.Stop();
}