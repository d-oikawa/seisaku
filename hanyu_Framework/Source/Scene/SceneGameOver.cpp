#include "SceneGameOver.h"

#include "Fwk/Framework.h"

//������
void SceneGameOver::Init() {
	//���̃V�[���̏����l���Ȃ��ɂ��Ă���
	mNextScene = SceneType::None;
}

//�I��
void SceneGameOver::Term() {
	//�Ȃ�
}

//�X�V
void SceneGameOver::Update() {
	//A�{�^�����������玟�̃V�[����Title��ݒ肷��
	if (Input_I->IsKeyDown('Z') || Input_I->IsButtonDown(0, GAMEPAD_BUTTON::A))
	{
		mNextScene = SceneType::Title;
	}
}

//�`��
void SceneGameOver::Render() {
	//�������`�悷��
	PrintText("�Q�[���I�[�o�[�E�E�E", 380.0f, 250.0f);
}
