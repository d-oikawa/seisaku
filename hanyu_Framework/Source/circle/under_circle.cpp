#include "under_circle.h"
#include <Fwk/Framework.h>
#include <AppDef.h>



//������
//�e�N�X�`���E�ʒu�E�R���W�����̐ݒ�
void Under_Circle::Init() {
	Decisioncircle::Init();
	{
		mTexture.Load("Images/rhythm/under.png");
		mSprite.Init();
		mSprite.SetTexture(mTexture);
		mSprite.SetPosition(250.0f, -WINDOW_HEIGHT / 1.5f);
	}
	{
		mCollision.SetTag("Under_circle");
		mCollision.SetCircle(64.0f, 64.0f, 32.0f);
	}
}

//�X�V
//�L�[���͒��̎��̂݃R���W�������A�N�e�B�u�ɂ��A�T�C�Y�������傫������
void Under_Circle::Update() {
	if (Input_I->IsKeyDown('J'))
	{
		mCollision.SetActive(true);
		mSoundSource.Play();
	}
	if (Input_I->IsKeyPressed('J'))
	{
		mSprite.SetSize(76.0f,76.0f);
	}
	else
	{
		mCollision.SetActive(false);
		mSprite.SetSize(64.0f, 64.0f);
	}
	Decisioncircle::Update();
}