#include "SceneGameClear.h"

#include "Fwk/Framework.h"

//������
void SceneGameClear::Init() {
	//���̃V�[���̏����l���Ȃ��ɂ��Ă���
	mNextScene = SceneType::None;
}

//�I��
void SceneGameClear::Term() {
	//�Ȃ�
}

//�X�V
void SceneGameClear::Update() {
	//A�{�^�����������玟�̃V�[����Title��ݒ肷��
	if (Input_I->IsKeyDown('Z') || Input_I->IsButtonDown(0, GAMEPAD_BUTTON::A))
	{
		mNextScene = SceneType::Title;
	}
}

//�`��
void SceneGameClear::Render() {
	//�������`�悷��
	PrintText("�Q�[���N���A�I", 380.0f, 250.0f);
}
