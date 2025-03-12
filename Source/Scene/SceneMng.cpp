#include "SceneMng.h"

#include "Fwk/Framework.h"

//�e�V�[���N���X�̃w�b�_
#include "SceneTitle.h"
#include "SceneInGame.h"
#include "SceneGameOver.h"
#include "SceneGameClear.h"

//������
void SceneMng::Init(SceneType startScene) {
	//���݂̃V�[���͂Ȃ�
	mCurrentScene = SceneType::None;
	//���ɓ������V�[�����L�^���Ă���
	mNextScene = startScene;

	//���݂̃V�[���N���X�̃|�C���^��null��ݒ肵�Ă���
	mpScene = nullptr;
}

//�I��
void SceneMng::Term() {
	//���݂̃V�[�����I������
	_endCurrentScene();
	//���̃V�[���͂Ȃ��Ƃ��Ă���
	mNextScene = SceneType::None;
}

//�X�V
void SceneMng::Update() {
	//���̃V�[�����ݒ肳��Ă���A
	//���݂̃V�[�������̃V�[���ƈقȂ��Ă�����V�[����؂�ւ���
	if (mNextScene!=SceneType::None&&mCurrentScene!=mNextScene)
	{
		//���̃V�[�����J�n����
		_beginScene(mNextScene);
	}
	//���݂̃V�[�����X�V
	if (mpScene!=nullptr)
	{
		mpScene->Update();
		mNextScene = mpScene->GetNextScene();
	}
}

//�`��
void SceneMng::Render() {
	//���݂̃V�[����`��
	if (mpScene != nullptr)
	{
		mpScene->Render();
	}
}

//�V�����V�[�����J�n����
void SceneMng::_beginScene(SceneType newScene) {
	//���ݓ����Ă���V�[�����~�߂�
	_endCurrentScene();

	//�V�����V�[�����J�n����
	switch (newScene)
	{
	case SceneType::Title:
		//�^�C�g���V�[���̐���
		mpScene = new SceneTitle();
		break;
	case SceneType::InGame:
		//�C���Q�[���V�[���̐���
		mpScene = new SceneInGame();
		break;
	case SceneType::GameOver:
		//�Q�[���I�[�o�[�V�[���̐���
		mpScene = new SceneGameOver();
		break;
	case SceneType::GameClear:
		//�Q�[���N���A�V�[���̐���
		mpScene = new SceneGameClear();
		break;
	}

	//���������V�[���̏�����
	if (mpScene != nullptr)
	{
		mpScene->Init();
	}

	//���݂̃V�[����ݒ�
	mCurrentScene = newScene;
}

//���݂̃V�[�����~����
void SceneMng::_endCurrentScene() {

	if (mpScene!=nullptr)
	{
		mpScene->Term();
		SAFE_DELETE(mpScene);
	}

	//�V�[�����~�߂��̂Ō��݂̃V�[�����Ȃ��ɂ��Ă���
	mCurrentScene = SceneType::None;
}