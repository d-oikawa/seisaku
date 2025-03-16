#include "GameObjectMng.h"

//RnderManerを使うのでinclude
#include "Fwk/Framework.h"

//画面サイズの定数を使うのでinclude
#include "AppDef.h"



Under_Circle mUnderCircle;
Upper_Circle mUpperCircle;
Beats mBeats;
NotesMng mNotesMng;

//全オブジェクト初期化
//各CSVファイルから・音楽・ノーツの情報を取得する
void GameObjectInit()
{
	{
		CSVFile csvFile;
		csvFile.Load("Data/Musicdata.csv");
		mBeats.CreateMusic(csvFile.GetCSVData());
	}
	{
		CSVFile csvFile;
		csvFile.Load("Data/Notesdata.csv");
		mNotesMng.CreateNotes(csvFile.GetCSVData());
	}

	mBeats.Init();
	mUnderCircle.Init();
	mUpperCircle.Init();

	

}

//全オブジェクト更新
void GameObjectUpdate()
{
	mUnderCircle.Update();
	mUpperCircle.Update();
	mBeats.Update();
}

//全オブジェクト描画
void GameObjectRender()
{
	mUnderCircle.Render();
	mUpperCircle.Render();
	mBeats.Render();
}

//全オブジェクト解放
void GameObjectTerm()
{
	mUnderCircle.Term();
	mUpperCircle.Term();
	mBeats.Term();
}


//Beatsクラスのポインタを返す
Beats* GetBeats()
{
	return &mBeats;
}