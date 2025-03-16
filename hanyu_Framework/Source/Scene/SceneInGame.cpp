#include "SceneInGame.h"

#include "Fwk/Framework.h"

#include "GameObjectMng/GameObjectMng.h"

#include "beats/Beats.h"

Beats pBeats;

//������
void SceneInGame::Init() {
	mNextScene = SceneType::None;
	GameObjectInit();
}

//�I��
void SceneInGame::Term() {
	//�Q�[���I�u�W�F�N�g�̉��
	GameObjectTerm();

}

//�X�V
//���y���Đ�����Ă��邩����
//�~�܂��Ă����玟�̃V�[���֑J��
void SceneInGame::Update() {
//�Q�[���I�u�W�F�N�g�̍X�V
	GameObjectUpdate();
	if (GetBeats()->IsPlaying())
	{
		mNextScene = SceneType::GameClear;
	}
	//�|�[�Y��ʂ֑J��
	if (Input_I->IsKeyDown('P')) {
		pBeats.StopBeats();
		mNextScene = SceneType::Pause;
	}
}

//�`��
void SceneInGame::Render() {
	//�Q�[���I�u�W�F�N�g�̕`��
	GameObjectRender();
}

float SceneInGame::GetTimer() {
	return mTimer;
}