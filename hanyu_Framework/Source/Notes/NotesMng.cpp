#include "NotesMng.h"

//SAFE_DELETEを使う為にインクルード
#include "Lib/LibDef.h"

//生成するノーツのクラスの宣言
#include "Under_Notes.h"
#include "Upper_Notes.h"

//Beatクラスのポインタを使うためinclude
#include "GameObjectMng/GameObjectMng.h"

//初期化
void NotesMng::Init() {
	//譜面作成時に使う変数の初期化
	mpBeatMapData = nullptr;
	mBeatNum = 0;
	mRowNum = 0;
	mBeat = 0;

	//アクティブなノーツのポインタ配列を空にしておく
	mActiveNotes.clear();
}

//後片付け
void NotesMng::Term() {
	//アクティブなノーツのポインタの配列を空にしておく
	mActiveNotes.clear();
	//全てのノーツプールを破棄
	DestroyPoolALL();
}

//更新
void NotesMng::Update() {
	//アクティブなノーツをすべて更新する
	for (int i = 0; i < mActiveNotes.size(); i++)
	{
		//更新
		mActiveNotes[i]->Update();
	}

	//更新の結果非アクティブとなった要素を取り除く
	for (int i = 0; i < mActiveNotes.size();)
	{
		if (mActiveNotes[i]->IsActive())
		{
			//mActiveNotes[i]がアクティブのままであれば
			//次の要素を処理するのでiをインクリメント
			++i;
		}
		else
		{
			//mActiveNotes[i]が非アクティブとなっていたらその要素を取り除く
			mActiveNotes.erase(mActiveNotes.begin() + i);
		}
	}

	//現在のBeatと更新されているBeatの値が同じかつこのビートで生成してあればreturn
	//出なければ生成フラグをおろす
	if (mBeat==GetBeats()->GetBeatsCounts()&&mIsCreateNotes)
	{
		return;
	}
	else
	{
		mIsCreateNotes = false;
	}


	//現在のBeatを取得
	mBeat = GetBeats()->GetBeatsCounts();

	//現在のBeatが生成するビートが同じ値であれば生成し、ループを抜ける 
	for (int i = 0; i < mRowNum; i++)
	{
		//拍数分繰り返し
		//iが0なら上ノーツ、1なら下ノーツの生成
		//ノーツが判定ラインに行く4拍前に生成するため+4にする
		for (int n = 0; n < mBeatNum; n++)
		{
			if (mpBeatMapData[i][mBeat])
			{
				CreateNotes(mNotesName[i],i);
				mIsCreateNotes = true;
				break;
			}
		}

		if (mIsCreateNotes)
		{
			break;
		}
	}
}

//描画
void NotesMng::Render() {
	//アクティブなノーツをすべて描画する
	for (int i = 0; i < mActiveNotes.size(); i++)
	{
		//更新
		mActiveNotes[i]->Render();
	}
}

//オブジェクトプールの生成
void NotesMng::GeneratePool(string notesName, int poolSize) {
	//下ノーツのプールの生成
	if (notesName == "Under_Notes")
	{
		//生成するノーツクラスを指定
		mNotesPools[notesName].Init<Under_Notes>(poolSize);
	}
	//上ノーツのプールの生成
	if (notesName == "Upper_Notes")
	{
		//生成するノーツクラスを指定
		mNotesPools[notesName].Init<Upper_Notes>(poolSize);
	}

}

//指定したノーツのオブジェクトプールの破棄
void NotesMng::DestroyPool(string notesName) {
	//ノーツのプール破棄
	if (mNotesPools.count(notesName) > 0)
	{
		mNotesPools[notesName].Term();
		mNotesPools.erase(notesName);
	}
}

//全てのノーツのオブジェクトプールの破棄
void NotesMng::DestroyPoolALL() {
	//下ノーツのバッファを破棄
	DestroyPool("Under_Notes");
	//上ノーツのバッファを破棄
	DestroyPool("Upper_Notes");
}

//ノーツの生成
void NotesMng::CreateNotes(string notesName,int notesline) {
	//指定のノーツプールから非アクティブのオブジェクトを取得する
	Notes* pNotes = nullptr;

	//ノーツプールから空きオブジェクトを取得
	if (mNotesPools.count(notesName) > 0)
	{
		pNotes = mNotesPools[notesName].Alloc();
	}

	//非アクティブのオブジェクトがなければ生成失敗
	if (pNotes == nullptr)
	{
		return;
	}
	//生成タイミングの時間を設定
	pNotes->SetBeat(mBeat);

	//生成するノーツの列を設定
	pNotes->SetNotesLine(notesline);

	//生成時に関数を呼び出す
	pNotes->OnCreated();

	//アクティブなノーツ配列に追加しておく
	mActiveNotes.push_back(pNotes);
}

//CSVデータからノーツを生成する
void NotesMng::CreateNotes(int beatNum, int rowNum, CSVData* pCsvData) {
	//nullptrが渡されてきたら何もしないで関数を抜ける
	if (pCsvData == nullptr)
	{
		return;
	}
	//拍数・行数を記録
	mBeatNum = beatNum;
	mRowNum = rowNum;


	//拍数×行数分の配列を確保する
	mpBeatMapData = new int* [mRowNum];
	for (int i = 0; i < mRowNum; i++)
	{
		mpBeatMapData[i] = new int[mBeatNum];
	}

	//行数分繰り返し
	for (int i = 0; i < mRowNum; i++)
	{
		//拍数分繰り返し
		for (int n = 0; n < mBeatNum; n++)
		{
			//n列i行目のタイルIDをCSVDataから取得
			int id = pCsvData->GetInt(n + (i * mBeatNum));
			//マップデータのn列i行目のタイルIDを上書き
			mpBeatMapData[i][n] = id;
		}
	}
}

//残りのノーツの数
int NotesMng::GetNotesCount() {
	return (int)mActiveNotes.size();
}
