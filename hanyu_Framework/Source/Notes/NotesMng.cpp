#include "NotesMng.h"

//SAFE_DELETEを使う為にインクルード
#include "Lib/LibDef.h"

//生成するノーツのクラスの宣言
#include "Under_Notes.h"
#include "Upper_Notes.h"

//初期化
void NotesMng::Init() {
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
			//mActiveEnemies[i]がアクティブのままであれば
			//次の要素を処理するのでiをインクリメント
			++i;
		}
		else
		{
			//mActiveEnemies[i]が非アクティブとなっていたらその要素を取り除く
			mActiveNotes.erase(mActiveNotes.begin() + i);
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
void NotesMng::CreateNotes(string notesName, float timer) {
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
	//時間を設定を設定
	pNotes->SetTimer(timer);

	//生成時に関数を呼び出す
	pNotes->OnCreated();

	//アクティブなノーツ配列に追加しておく
	mActiveNotes.push_back(pNotes);
}

//CSVデータからノーツを生成する
void NotesMng::CreateNotes(CSVData* pCsvData) {
	//nullptrが渡されてきたら何もしないで関数を抜ける
	if (pCsvData == nullptr)
	{
		return;
	}

	//ノーツの情報は、名前、表示する秒数の2つで１組。
	//CSVファイルの全要素を2で割ってノーツの配置情報の数とする
	int notesNum = pCsvData->GetElementCount() / 2;
	//配置するノーツの数だけ繰り返し
	for (int i = 0; i < notesNum; i++)
	{
		//ノーツ１体の配置データの先頭の要素番号
		int index = i * 2;
		//ノーツの名前を取得
		string notesName;
		pCsvData->GetString(index, &notesName);
		//配置先の座標を取得
		float timer;
		timer = pCsvData->GetFloat(index + 1);

		//ノーツを生成
		CreateNotes(notesName, timer);
	}
}

//残りのノーツの数
int NotesMng::GetNotesCount() {
	return (int)mActiveNotes.size();
}
