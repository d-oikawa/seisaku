#pragma once

//Scene�N���X���g���̂�include
#include "Scene.h"

//�V�[���Ǘ��N���X
class SceneMng
{
public:
	//������
	void Init(SceneType startScene);
	//��Еt��
	void Term();
	//�X�V
	void Update();
	//�`��
	void Render();

private:
	//�V�����V�[�����J�n����
	void _beginScene(SceneType newScene);
	//���݂̃V�[�����I������
	void _endCurrentScene();

private:
	//���݂̃V�[��
	SceneType mCurrentScene;
	//���̃V�[��
	SceneType mNextScene;
	//�e�V�[���N���X�̃|�C���^
	Scene* mpScene;
};
