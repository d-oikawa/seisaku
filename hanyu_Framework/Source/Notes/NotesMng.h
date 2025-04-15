#pragma once

#include <string>
#include <vector>
#include <map>
using namespace std;

#include "Lib/Math/Vector.h"
using namespace Lib::Math;

//CSVデータを使うのでCSVFile.hを取り込む
#include "Fwk/File/CSVFile.h"

//生成するノーツのクラスの宣言を取り込む
#include "Notes.h"

//ノーツプールクラスを使うので
#include "NotesPool.h"

//ノーツ管理クラス
class  NotesMng
{
public:
	//初期化
	void Init();
	//後片付け
	void Term();
	//更新
	void Update();
	//描画
	void Render();

	//オブジェクトプールの生成
	void GeneratePool(string notesName, int poolSize);
	//指定したノーツのオブジェクトプールの開放
	void DestroyPool(string notesName);
	//すべてのノーツのオブジェクトプールの開放
	void DestroyPoolALL();

	//ノーツの生成 
	void CreateNotes(string notesName,int notesline);

	//残りのノーツの数
	int GetNotesCount();

	//CSVデータからノーツを生成する
	void CreateNotes(int beatNum, int rowNum, CSVData* pCsvData);

	//ノーツの生成フラグを取得
	bool IsCreateNotes() { return mIsCreateNotes; }

private:
	//アクティブなノーツオブジェクトのポインタ配列
	vector<Notes*> mActiveNotes;
	//ノーツ種別ごとのオブジェクトプール
	map<string, NotesPool>mNotesPools;
	//現在のビートを取得
	int mBeat;
	//ノーツのタグの設定
	string mNotesName[2] = { "Upper_Notes","Under_Notes" };

private:

	//拍数と行数を保存するための値
	int mBeatNum;
	int mRowNum;

	//譜面のデータ(id)を収める配列のアドレスの作成
	int** mpBeatMapData;

	//ノーツが生成されているか
	bool mIsCreateNotes;
};

