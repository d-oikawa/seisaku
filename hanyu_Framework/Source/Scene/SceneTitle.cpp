#include "SceneTitle.h"

#include "Fwk/Framework.h"

//������
void SceneTitle::Init() {
	//���̃V�[���̏����l���Ȃ��ɂ��Ă���
	mNextScene = SceneType::None;
}

//�I��
void SceneTitle::Term() {
	//�Ȃ�
}

//�X�V
void SceneTitle::Update() {
	//A�{�^�����������玟�̃V�[����InGame��ݒ肷��
	if (Input_I->IsKeyDown('Z')||Input_I->IsButtonDown(0,GAMEPAD_BUTTON::A))
	{
		mNextScene = SceneType::InGame;
	}
}

//�`��
void SceneTitle::Render() {
	//�������`�悷��
	PrintText("aaaaa", 380.0f, 250.0f);
}
