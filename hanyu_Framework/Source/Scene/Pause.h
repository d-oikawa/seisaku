#pragma once

//Scene�N���X���p������̂�include
#include "Scene.h"

//�����N���X�̒�`��include
#include "Fwk/Audio/SoundSource.h"
//MP3�t�@�C���������N���X��include
#include "Fwk/Audio/MP3Sound.h"

//Sprite��Texture,Animation���g�����߂̃w�b�_�t�@�C��
#include "Fwk/Graphics.h"

//Vector2f�N���X���g�����߂̃w�b�_�t�@�C��
#include "Lib/Math/Vector.h"

//�^�C�g���V�[���N���X
class Pause : public Scene
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

private:
	//BGM�ǂݍ��ݗp
	MP3Sound mSound;
	//����
	SoundSource mSoundSource;

	Texture yariTexture;
	Texture conTexture;
	Sprite yariSprite;
	Sprite conSprite;
	Vector2f mPosition;

	float X;
	bool i = false;
};