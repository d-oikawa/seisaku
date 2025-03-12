#include "Notes.h"
#include "GameDef.h"
#include "Fwk/Framework.h"
#include "Scene/SceneInGame.h"

SceneInGame mInGame;

//������
//���ʂ���T�C�Y�E�R���W�����E�T�E���h�E�A�j���[�V�����̐ݒ�
void Notes::Init() {
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
		mSound.Load("Sound/jumpC.wav");
		mSoundSource.Init(mSound);
	}
	_initAnimation();
}

//�X�V
void Notes::Update() {
	mPosition.x -= 105.0f * Time_I->GetDeltaTime();
	mSprite.Update();
}

//�`��
void Notes::Render() {
	mSprite.Draw();
}

//���
//�e�N�X�`���E�R���W�����E���y�̉��
void Notes::Term() {
	mTexture.Unload();
	mSprite.Term();
	CollisionManager_I->Unregister(&mCollision);
	mSound.Unload();
	mSoundSource.Term();
}

//���݈ʒu���擾
Vector2f Notes::GetPosition() {
	return mPosition;
}

//�m�[�c����]����A�j���[�V�������쐬
void Notes::_initAnimation() {
	float rotDeg[] = {
		0.0f,
		360.0f,
	};
	Animation animtrun;
	CreateAnimationRotation(animtrun, "trun", 2, 0.5, true, rotDeg);
	mSprite.AddAnimation(animtrun);
	mSprite.PlayAnimation("trun");
}

//�A�N�e�B�u���ۂ���Ԃ�
bool Notes::IsActive() {
	return mIsActive;
}

//�A�N�e�B�u��Ԃ̐ݒ�
void Notes::SetActive(bool isActive) {
	mIsActive = isActive;
	mCollision.SetActive(isActive);
}

//�^�C�}�[��ݒ�
void Notes::SetTimer(float timer) {
	mTimer = mInGame.GetTimer();
}

//�^�C�}�[���擾
float Notes::GetTimer() {
	return mTimer;
}

void Notes::OnCreated() {
	//�Ȃ�
}