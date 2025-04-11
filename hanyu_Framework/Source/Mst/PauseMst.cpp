#include "PauseMst.h"

//CSVファイルを使うので
#include "Fwk/File/CSVFile.h"

//ポーズ情報マスタの変数
PauseDataMst pauseDataMst;

//ポーズ情報マスタを取得する
PauseDataMst& GetPauseDataMst() {
	return pauseDataMst;
}

//初期化
void PauseDataMst::Init() {
	//データをクリアしておく
	mData.clear();

	//ポーズマスタを読み込む
	CSVFile pauseMst;
	pauseMst.Load("Data/PauseData.csv");

	//データ数/3がポーズの中の内容
	int pauseNum = pauseMst.GetElementCount() / 3;
	//読み込み
	for (int i = 0; i < pauseNum; ++i) {
		int n = i * 3;
		PauseData newElem;
		//選択名
		pauseMst.GetString(n, &newElem.Name);
		//データを追加
		mData.push_back(newElem);
	}
}

//終了
void PauseDataMst::Term() {
	//データをクリアしておく
	mData.clear();
}

//データを返す
int PauseDataMst::Size() {
	return mData.size();
}

//指定した番号のポーズデータを返す
PauseData* PauseDataMst::Get(int index) {
	//不正な番号が指定された場合はnullptrを返す
	if (index < 0 || index >= mData.size()) {
		return nullptr;
	}
	return &mData[index];
}