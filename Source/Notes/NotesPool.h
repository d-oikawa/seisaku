#pragma once

//敵プールクラス

//可変表記配列std::vectorを使う
#include <vector>

#include "Notes.h"

class NotesPool
{
public:
	//初期化
	//class Tは、Init関数の呼び出し時に具体的な敵クラスに置き換わる。
	template<class T>
	void Init(int poolSize) {
		//プール全要素分の敵オブジェクトを生成
		for (int i = 0; i < poolSize; ++i) {
			//指定された敵クラス変数を1つ生成
			//Notes+にキャストできるクラスが指定されるのが前提
			Notes* pNewNotes = new T();
			//初期化
			pNewNotes->Init();
			//最初は非アクティブ
			pNewNotes->SetActive(false);
			//プールにアドレスを記録しておく
			mPool.push_back(pNewNotes);
		}
	}
	//後片付け
	void Term();
	//オブジェクトを１つ活性化して戻す
	Notes* Alloc();

protected:
	//Notes*型の可変長配列
	std::vector<Notes*>mPool;
};