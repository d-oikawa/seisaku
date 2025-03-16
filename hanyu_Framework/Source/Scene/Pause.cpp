#include "Pause.h"

#include "SceneInGame.h"

#include "beats/Beats.h"

#include "Fwk/Framework.h"

#include <iostream>
#include <iomanip>


Beats pBeats2;

//������
void Pause::Init() {
	//���̃V�[���̏����l���Ȃ��ɂ��Ă���
	mNextScene = SceneType::None;
	//�e�N�X�`���̓ǂݍ���
	conTexture.Load("Images/2dAction/con.png");
	yariTexture.Load("Images/2dAction/yari.png");

	//�X�v���C�g�̏�����
	conSprite.Init();
	yariSprite.Init();
	//�e�N�X�`���̐ݒ�
	conSprite.SetTexture(conTexture);
	yariSprite.SetTexture(yariTexture);
	//�X�v���C�g�̃T�C�Y�ݒ�
	conSprite.SetSize(240.0f, 200.0f);
	yariSprite.SetSize(240.0f, 200.0f);
	//YajirushiSprite.SetPivot(Pivot::TopLeft);
	conSprite.SetPosition(480.0f, -250.0f);
	yariSprite.SetPosition(480.0f, -450.0f);
}

//�I��
void Pause::Term() {
	//�Ȃ�
	conSprite.Term();
	yariSprite.Term();
	conTexture.Unload();
	yariTexture.Unload();
}

//�X�V
void Pause::Update() {
	mSoundSource.Stop();
	//PrintText("�R���e�B�j���[", 420.0f, 50.0f);
	//PrintText("�ŏ������蒼��", 400.0f, 250.0f);
		if (i == false) {
			conSprite.SetAdditionalColor(10.0f, 0.0f, 0.0f);
			yariSprite.SetAdditionalColor(0.0f, 0.0f, 0.0f);
			i = true;
		}
		if (Input_I->IsKeyDown('2')) {
			yariSprite.SetAdditionalColor(1.0f, 0.0f, 0.0f);
			conSprite.SetAdditionalColor(0.0f, 0.0f, 0.0f);
		}
		if (Input_I->IsKeyDown('8')) {
			conSprite.SetAdditionalColor(1.0f, 7.0f, 0.0f);
			yariSprite.SetAdditionalColor(0.0f, 0.0f, 0.0f);
		}
		else  {
			if (Input_I->IsKeyDown('5')) {
				mNextScene = SceneType::Title;
			}
		}
}

//�`��
void Pause::Render() {
	conSprite.Draw();
	yariSprite.Draw();
}