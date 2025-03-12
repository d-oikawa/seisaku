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
	void CreateNotes(string notesName, float timer);

	//残りのノーツの数
	int GetNotesCount();

	//CSVデータからノーツを生成する
	void CreateNotes(CSVData* pCsvData);

private:
	//アクティブなノーツオブジェクトのポインタ配列
	vector<Notes*> mActiveNotes;
	//ノーツ種別ごとのオブジェクトプール
	map<string, NotesPool>mNotesPools;
};

