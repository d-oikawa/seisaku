#include "NotesPool.h"

//SAFE_DELETEを使いたいので#include
#include "Fwk/Framework.h"

//後片付け
void NotesPool::Term() {
	//Notes*配列の各要素にnewされた敵のオブジェクトのアドレスが
	//記録されているので、1つ１つDELETEしていく
	for (int i = 0; i < mPool.size(); i++)
	{
		SAFE_DELETE(mPool[i]);
	}
	//プール破棄
	mPool.clear();
}

//オブジェクトを１つ活性化して戻す
Notes* NotesPool::Alloc() {
	//戻り値を記録する変数
	Notes* pResult = nullptr;

	for (int i = 0; i < mPool.size(); i++)
	{
		//非アクティブのオブジェクトを見つけたら活性化して戻り値にする。
		if (mPool[i]->IsActive() == false)
		{
			//戻り値にする
			pResult = mPool[i];
			//活性化しておく
			pResult->SetActive(true);
			//ループは抜けてよい
			break;
		}
	}
	return pResult;
}