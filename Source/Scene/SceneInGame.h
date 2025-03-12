#pragma once

//Scene�N���X���p������̂�include
#include "Scene.h"

//�����N���X�̒�`��include
#include "Fwk/Audio/SoundSource.h"
//MP3�t�@�C���������N���X��include
#include "Fwk/Audio/MP3Sound.h"

//�^�C�g���V�[���N���X
class SceneInGame : public Scene
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

	//���Ԃ�Ԃ�
	float GetTimer();

private:
	//BGM�ǂݍ��ݗp
	MP3Sound mSound;
	//����
	SoundSource mSoundSource;
	//�^�C�}�[
	float mTimer;
};