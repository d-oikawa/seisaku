#include "Fwk/Collision/Collider.h"

namespace Fwk {
	namespace Collision {

		Collider::Collider()
			: mShape({ ShapeType::None })
			, mPosition(0.0f, 0.0f)
			, mTag()
			, mIsActive(true)
			, mActiveDuration(ActiveDuration::Default)
			, mIsCollide(false)
			, mOnOverlapped(nullptr)
			, mOnOverlappedEx(nullptr)
			, mpOwner(nullptr)
			, mGroup(0)
			, mHitGroup(0)
		{
			;
		}

		Collider::~Collider()
		{
			if (mOnDestroyed != nullptr) {
				mOnDestroyed(this);
			}
		}

		void Collider::SetPosition(float _x, float _y) {
			mPosition = { _x, _y };
		}
		void Collider::SetPosition(const Vector2f& _position) {
			mPosition = _position;
		}

		//中心位置の取得
		const Vector2f& Collider::GetPosition()const {
			return mPosition;
		}

		void Collider::SetCircle(const Circle& _circle) {
			mShape.circle = _circle;
			mShape.shapeType = ShapeType::Circle;
		}
		void Collider::SetCircle(const Vector2f& _position, float _r) {
			mShape.circle.position = _position;
			mShape.circle.radius = _r;
			mShape.shapeType = ShapeType::Circle;
		}
		void Collider::SetCircle(float _x, float _y, float _r) {
			mShape.circle.position = { _x,_y };
			mShape.circle.radius = _r;
			mShape.shapeType = ShapeType::Circle;
		}

		void Collider::SetRect(const Rect& _rect) {
			mShape.rect = _rect;
			mShape.shapeType = ShapeType::Rect;
		}
		void Collider::SetRect(const Vector2f& _position, float _width, float _height) {
			mShape.rect.position = _position;
			mShape.rect.width = _width;
			mShape.rect.height = _height;
			mShape.shapeType = ShapeType::Rect;
		}
		void Collider::SetRect(float _x, float _y, float _width, float _height) {
			mShape.rect.position = { _x,_y };
			mShape.rect.width = _width;
			mShape.rect.height = _height;
			mShape.shapeType = ShapeType::Rect;
		}

		//形状の取得
		ShapeType Collider::GetShapeType()const {
			return mShape.shapeType;
		}
		const Circle& Collider::GetShapeCircle()const {
			return mShape.circle;
		}
		const Rect& Collider::GetShapeRect()const {
			return mShape.rect;
		}

		bool Collider::IsOverlapped(Collider& Collider)const {

			Shape colA = mShape;
			Shape colB = Collider.mShape;

			bool isHit = false;

			//無効設定のコリジョンは処理しない
			if (mIsActive == false || Collider.mIsActive == false) {
				return false;
			}

			//コリジョン形状がないものは処理対象外
			if (colA.shapeType == ShapeType::None || colB.shapeType == ShapeType::None) {
				return false;
			}

			//形状によって衝突判定関数を変える
			if (colA.shapeType == ShapeType::Circle && colB.shapeType == ShapeType::Circle) {
				colA.circle.position += mPosition;
				colB.circle.position = Collider.mPosition;
				//円と円の衝突
				isHit = IsCirclesOverlapped(colA.circle, colB.circle);
			}
			else if (colA.shapeType == ShapeType::Rect && colB.shapeType == ShapeType::Rect) {
				colA.rect.position += mPosition;
				colB.rect.position = Collider.mPosition;
				//矩形と矩形の衝突
				isHit = IsRectsOverlapped(colA.rect, colB.rect);
			}
			else if (colA.shapeType == ShapeType::Circle && colB.shapeType == ShapeType::Rect) {
				colA.circle.position += mPosition;
				colB.rect.position = Collider.mPosition;
				//Aが円,Bが矩形の衝突
				isHit = IsCircleAndRectOverlapped(colA.circle, colB.rect);
			}
			else {
				colA.rect.position += mPosition;
				colB.circle.position = Collider.mPosition;
				//Aが矩形,Bが円の衝突
				isHit = IsCircleAndRectOverlapped(colB.circle, colA.rect);
			}

			return isHit;
		}

		//名前の設定
		void Collider::SetName(const std::string& _name) {
			mName = _name;
		}
		const std::string& Collider::GetName()const {
			return mName;
		}

		void Collider::SetTag(const std::string& _tag) {
			mTag = _tag;
		}
		const std::string& Collider::GetTag()const {
			return mTag;
		}

		void Collider::SetActive(bool _isActive) {
			mIsActive = _isActive;
		}

		bool Collider::IsActive()const {
			return mIsActive;
		}

		void Collider::SetOwner(void* _pOwner) {
			mpOwner = _pOwner;
		}

		void* Collider::GetOwner()const {
			return mpOwner;
		}

		void Collider::SetCallbackFunction(const std::function<void(const Collider& colA, const Collider& colB)>& _OnOverlapped) {
			mOnOverlapped = _OnOverlapped;
		}

		void Collider::SetCallbackFunctionEx(const std::function<void(const CollisionEvent& CollisionEvent)>& _OnOverlapped) {
			mOnOverlappedEx = _OnOverlapped;
		}

		//グループの設定
		void Collider::SetGroup(int _group) {
			mGroup = _group;
		}

		//グループの解除
		void Collider::ClearGroup() {
			mGroup = 0;
		}

		//グループの取得
		unsigned int Collider::GetGroup()const {
			return mGroup;
		}

		//衝突対象グループの追加
		void Collider::AddHitGroup(int _hitGroup) {
			mHitGroup |= (1 << _hitGroup);
		}
		//衝突対象グループの削除
		void Collider::RemoveHitGroup(int _hitGroup) {
			mHitGroup &= ~(1 << _hitGroup);

		}
		//衝突対象グループのクリア
		void Collider::ClearHitGroup() {
			mHitGroup = 0;

		}
		//衝突対象グループの取得
		unsigned int Collider::GetHitGroup()const {
			return mHitGroup;
		}

		//活性期間の設定
		void Collider::SetActiveDuration(ActiveDuration _activationDuration) {
			mActiveDuration = _activationDuration;
		}

	}
}
