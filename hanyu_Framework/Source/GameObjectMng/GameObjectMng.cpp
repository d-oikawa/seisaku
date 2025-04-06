#include "GameObjectMng.h"
//Key-Valueファイルの読み込み
#include "Fwk/File/KeyValueFile.h"

//RnderManerを使うのでinclude
#include "Fwk/Framework.h"

//画面サイズの定数を使うのでinclude
#include "AppDef.h"

//circleクラスのinclude
#include "circle/circleMng.h"


CircleMng mCircleMng;
NotesMng mNotesMng;
Beats mBeats;

//全オブジェクト初期化
//各CSVファイルから・音楽・ノーツの情報を取得する
void GameObjectInit()
{
	mNotesMng.Init();

	//オブジェクトプールの設定
	mNotesMng.GeneratePool("Under_Notes", 20);
	mNotesMng.GeneratePool("Upper_Notes", 20);

	//音楽の情報の取得
	{
		CSVFile csvFile;
		csvFile.Load("Data/Musicdata.csv");
		mBeats.CreateMusic(csvFile.GetCSVData());
	}
	mBeats.Init();

	//譜面の情報の取得
	{
		KeyValueFile BeatMapFile;
		BeatMapFile.Load("Data/Notesdata.csv");
		//譜面の拍数を取得
		int col = BeatMapFile.GetInt("Beat");
		//譜面の行数を取得
		int row = BeatMapFile.GetInt("Row");
		//CSVDataから譜面を生成する
		mNotesMng.CreateNotes(col, row, BeatMapFile.GetCSVData("notes"));
	}
	mCircleMng.Init();
}

//全オブジェクト更新
void GameObjectUpdate()
{
	mNotesMng.Update();
	mBeats.Update();
	mCircleMng.Update();
}

//全オブジェクト描画
void GameObjectRender()
{
	mNotesMng.Render();
	mBeats.Render();
	mCircleMng.Render();
}

//全オブジェクト解放
void GameObjectTerm()
{
	mNotesMng.Term();
	mBeats.Term();
	mCircleMng.Term();
}


//Beatsクラスのポインタを返す
Beats* GetBeats()
{
	return &mBeats;
}