#pragma once

//Scene�N���X���p������̂�include
#include "Scene.h"

//�^�C�g���V�[���N���X
class SceneTitle : public Scene
{
public:
	//������
	void Init();
	//��Еt��
	void Term();
	//�X�V
	void Update();
	//�`��
	void Render();
};