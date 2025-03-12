﻿#include "Fwk/Collision/CollisionManager.h"

#include "Lib/LibDef.h"

namespace Fwk {
namespace Collision {


void CollisionManager::Element::InsertNext(Element* p) {
	if (p == nullptr) {
		return;
	}

	p->pNext = pNext;
	p->pPrev = this;
	if (pNext) {
		pNext->pPrev = p;
	}
	pNext = p;
}

void CollisionManager::Element::InsertPrev(Element* p) {

	if (p == nullptr) {
		return;
	}

	p->pPrev = pPrev;
	p->pNext = this;

	if (pPrev) {
		pPrev->pNext = p;
	}
	pPrev = p;
}

void CollisionManager::Element::Remove() {
	if (pPrev) {
		pPrev->pNext = pNext;
	}
	if (pNext) {
		pNext->pPrev = pPrev;
	}
	pPrev = nullptr;
	pNext = nullptr;
}

CollisionManager::CollisionManager()
	: mpBuffer(nullptr)
	, mBufferSize(0)
	, mUsingTop()
	, mUsingTail()
	, mFreeTop()
	, mFreeTail()
{
	;
}

CollisionManager::~CollisionManager()
{
	Term();
}

void CollisionManager::Init(int BufferSize)
{
	Term();

	if (BufferSize < 1) {
		return;
	}
	
	mUsingTop.pPrev = nullptr;
	mUsingTop.pNext= &mUsingTail;

	mUsingTail.pPrev = &mUsingTop;
	mUsingTail.pNext = nullptr;

	mFreeTop.pPrev = nullptr;
	mFreeTop.pNext = &mFreeTail;

	mFreeTail.pPrev = &mFreeTop;
	mFreeTail.pNext = nullptr;

	mpBuffer = new Element[BufferSize];
	mBufferSize = BufferSize;

	Element* p = &mFreeTop;

	for (int i = 0; i < BufferSize; ++i) {
		mpBuffer[i].pCollision = nullptr;
		mpBuffer[i].pNext = nullptr;
		mpBuffer[i].pPrev = nullptr;
		mpBuffer[i].bPendingRemove = false;

		p->InsertNext(&mpBuffer[i]);
		p = &mpBuffer[i];
	}
}

void CollisionManager::Term()
{
	_procPendingRemoveElements();

	if (!mOverlappedCollisions.empty()) {
		for (auto& p : mOverlappedCollisions){
			SAFE_DELETE(p.second);
		}
		mOverlappedCollisions.clear();
	}

	for (int i = 0; i < mBufferSize; ++i) {
		if (mpBuffer[i].pCollision) {
			mpBuffer[i].pCollision->mOnDestroyed = nullptr;
		} 
	}
	SAFE_DELETE_ARRAY(mpBuffer);

	mBufferSize = 0;
}

void CollisionManager::Register(Collision* pCollision)
{
	if (pCollision == nullptr) {
		return;
	}

	//既に登録されてたらまずいので重複チェック
	if (_findElement(pCollision) != nullptr) {
		return;
	}
	//フリーの要素をひとつ取り出す
	Element* p = mFreeTop.pNext;
	if (p == &mFreeTail) {
		return;
	}
	//フリーのリストから外して
	p->Remove();
	//使用中リストの一番最後に挿入
	mUsingTail.InsertPrev(p);
	//コリジョンを登録しておく
	p->pCollision = pCollision;

	p->pCollision->mOnDestroyed = std::bind(&CollisionManager::Unregister, this, std::placeholders::_1);

	mOverlappedCollisions[p] = new std::map<Element*,OverlapInfo>();
}

void CollisionManager::Unregister(Collision* pCollision)
{
	if (pCollision == nullptr) {
		return;
	}

	//登録されていなければ何もしない
	Element* p = _findElement(pCollision);
	if (p==nullptr) {
		return;
	}

	p->pCollision->mOnDestroyed = nullptr;

	//破棄待ち状態にしておく。
	p->bPendingRemove = true;
}

void CollisionManager::ProcCollisions() {

	//コリジョンが衝突していた時の処理
	const auto _onOverlapped = [&](Element* _p0, Element* _p1) 
		{
			//破棄待ちのオブジェクトがあったら処理しない
			if (_p0->bPendingRemove || _p1->bPendingRemove) {
				return;
			}

			if (_p0->pCollision->mOnOverlapped != nullptr) {
				_p0->pCollision->mOnOverlapped(*_p0->pCollision, *_p1->pCollision);
			}

			_p0->pCollision->mIsCollide = true;

			if (mOverlappedCollisions[_p0]->count(_p1) == 0
			||  (*mOverlappedCollisions[_p0])[_p1].eventType == CollisionEventType::Exit ) {

				(*mOverlappedCollisions[_p0])[_p1].eventType = CollisionEventType::Enter;

				if (_p0->pCollision->mOnOverlappedEx != nullptr) {

					CollisionEvent e = {
						_p0->pCollision,
						_p1->pCollision,
						CollisionEventType::Enter
					};

					_p0->pCollision->mOnOverlappedEx(e);
				}
			}
			else 
			{
				(*mOverlappedCollisions[_p0])[_p1].eventType = CollisionEventType::Stay;

				if (_p0->pCollision->mOnOverlappedEx != nullptr) {

					CollisionEvent e = {
						_p0->pCollision,
						_p1->pCollision,
						CollisionEventType::Stay
					};

					_p0->pCollision->mOnOverlappedEx(e);
				}
			}
			
			(*mOverlappedCollisions[_p0])[_p1].isCollide = true;
		};

	for (auto& p0 : mOverlappedCollisions){
		for (auto& p1 : *p0.second) {
			p1.second.isCollide = false;
		}
	}

	Element* p0 = mUsingTop.pNext;
	
	while (p0 != &mUsingTail) {

		//廃棄待ちのオブジェクトは処理しない
		if (p0->bPendingRemove) {
			p0 = p0->pNext;
			continue;
		}

		Collision* pColA = p0->pCollision;

		//有効でなければスキップ
		if (!pColA->IsActive()) {
			p0 = p0->pNext;
			continue;
		}

		// 衝突グループを取得
		unsigned int groupA = (1 << pColA->GetGroup());
		// 衝突対象グループを取得
		unsigned int hitGroupA = pColA->GetHitGroup();

		Element* p1 = p0->pNext;

		while (p1 != &mUsingTail) {

			//廃棄待ちのオブジェクトは処理しない
			if (p1->bPendingRemove) {
				p1 = p1->pNext;
				continue;
			}

			Collision* pColB = p1->pCollision;

			//有効でなければスキップ
			if (!pColB->IsActive()) {
				p1 = p1->pNext;
				continue;
			}

			unsigned int groupB = ( 1 << pColB->GetGroup() );
			unsigned int hitGroupB = pColB->GetHitGroup();

			//A→B, B→Aいずれも衝突対象でなければ次の要素に
			if ((hitGroupA & groupB) == 0 && (hitGroupB & groupA) == 0) {
				p1 = p1->pNext;
				continue;
			}

			//二つのコリジョンの衝突チェック
			if (pColA->IsOverlapped(*pColB)) {
				//AにBがぶつかる対象であればコールバック
				if ((hitGroupA & groupB) != 0) {
					_onOverlapped(p0, p1);
				}
				//BにAがぶつかる対象であればコールバック
				if ((hitGroupB & groupA) != 0) {
					_onOverlapped(p1, p0);
				}
			}

			p1 = p1->pNext;
		}

		p0 = p0->pNext;
	}

	for (auto& p0 : mOverlappedCollisions){

		if (p0.first->bPendingRemove) {
			continue;
		}

		for (auto& p1 : *p0.second) {

			if (p1.first->bPendingRemove) {
				continue;
			}

			if ((p1.second.eventType == CollisionEventType::Enter || p1.second.eventType == CollisionEventType::Stay)
				&& !p1.second.isCollide)
			{
				p1.second.eventType = CollisionEventType::Exit;
							
				if (p0.first->pCollision->mOnOverlappedEx != nullptr) {

					CollisionEvent e = {
						p0.first->pCollision, 
						p1.first->pCollision,
						CollisionEventType::Exit
					};

					p0.first->pCollision->mOnOverlappedEx(e);
				}
			}
		}
	}

	//ヒット済フラグを下げつつ、一度当たったら非アクティブにするコリジョンを処理する
	p0 = mUsingTop.pNext;
	while (p0 != &mUsingTail) {
		if(!p0->bPendingRemove && p0->pCollision->mIsCollide){
			//一度当たったら非アクティブにする設定で、かつ、
			//衝突していたら
			if (p0->pCollision->mActiveDuration == ActiveDuration::Once) {
				//非アクティブに設定
				p0->pCollision->SetActive(false);
			}
			p0->pCollision->mIsCollide = false;
		}
		p0 = p0->pNext;
	}

	//破棄待ちの要素を処理する
	_procPendingRemoveElements();

}


CollisionManager::Element* CollisionManager::_findElement(Collision* pCollision) {
	Element* p = mUsingTop.pNext;
	while (p != &mUsingTail) {
		if (p->pCollision == pCollision) {
			return p;
		}
		p = p->pNext;
	}
	return nullptr;
}

void CollisionManager::_procPendingRemoveElements() {

	if (mBufferSize == 0 || mpBuffer == nullptr) {
		return;
	}

	const auto _onRemove = [&](Element* p)
	{
		p->Remove();
		mFreeTop.InsertNext(p);

		SAFE_DELETE(mOverlappedCollisions[p]);
		mOverlappedCollisions.erase(p);
		for (auto& pair : mOverlappedCollisions){
			if (pair.second->count(p) > 0) {
				pair.second->erase(p);
			}
		}

		p->pCollision = nullptr;
		p->bPendingRemove = false;

	};

	Element* pElem = mUsingTop.pNext;

	if (pElem == nullptr) {
		return;
	}

	while (pElem != &mUsingTail) {

		Element* pNext = pElem->pNext;

		if (pElem->bPendingRemove) {
			_onRemove(pElem);
		}

		pElem = pNext;
	}
}

}
}
