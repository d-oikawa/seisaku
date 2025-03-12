#pragma once

#include <map>
#include "Collision.h"
#include "CollisionTypes.h"

using namespace Lib::Math;
using namespace std;

namespace Fwk {
namespace Collision {

class CollisionManager
{
	struct Element {
		Collision* pCollision;
		bool bPendingRemove;
		Element* pPrev;
		Element* pNext;

		void InsertNext(Element* p);
		void InsertPrev(Element* p);
		void Remove();
	};


	struct OverlapInfo{
		CollisionEventType eventType;
		bool isCollide;
	};

public:

	CollisionManager();
	~CollisionManager();

	void Init(int BufferSize);

	void Term();

	void Register(Collision* pCollison);
	void Unregister(Collision* pCollison);

	void ProcCollisions();

private:

	Element* _findElement(Collision* pCollision);

	void _procPendingRemoveElements();

private:

	Element* mpBuffer;
	int mBufferSize;

	Element mUsingTop;
	Element mUsingTail;

	Element mFreeTop;
	Element mFreeTail;

	std::map<Element*, std::map<Element*,OverlapInfo>*> mOverlappedCollisions;
};

}
}
