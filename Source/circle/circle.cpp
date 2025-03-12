#include "circle.h"
#include "GameDef.h"
#include "Fwk/Framework.h"

//������
//���ʂ���T�C�Y�E�R���W�����E�T�E���h�E�A�j���[�V�����̐ݒ�
void Decisioncircle::Init() {
	{
		mSprite.SetSize(64.0f, 64.0f);
	}
	{
		mCollision.SetGroup((int)CollisionGroup::decisioncircle);
		mCollision.AddHitGroup((int)CollisionGroup::notes);
		mCollision.SetOwner(this);
		mCollision.SetActive(true);
		CollisionManager_I->Register(&mCollision);
	}
	{
		mSound.Load("Sound/hit.wav");
		mSoundSource.Init(mSound);
	}
	_initAnimation();
}

//�X�V
void Decisioncircle::Update() {
	mSprite.Update();
}

//�`��
void Decisioncircle::Render() {
	mSprite.Draw();
}

//���
//�e�N�X�`���E�R���W�����E���y�̉��
void Decisioncircle::Term() {
	mTexture.Unload();
	mSprite.Term();
	CollisionManager_I->Unregister(&mCollision);
	mSound.Unload();
	mSoundSource.Term();
}

//���݈ʒu���擾
Vector2f Decisioncircle::GetPosition() {
	return mPosition;
}

//����T�[�N������]����A�j���[�V�������쐬
void Decisioncircle::_initAnimation() {
	float rotDeg[] = {
		0.0f,
		360.0f,
	};
	Animation animtrun;
	CreateAnimationRotation(animtrun, "trun", 2, 0.5, true, rotDeg);
	mSprite.AddAnimation(animtrun);
	mSprite.PlayAnimation("trun");
}