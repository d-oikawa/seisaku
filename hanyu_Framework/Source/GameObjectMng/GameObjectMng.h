#pragma once

#include "circle/under_circle.h"
#include "circle/upper_circle.h"

//���y���Đ�������
#include "beats/Beats.h"

//�m�[�c�̏����擾����
#include "Notes/NotesMng.h"

//�S�I�u�W�F�N�g������
void GameObjectInit();

//�S�I�u�W�F�N�g�X�V
void GameObjectUpdate();

//�S�I�u�W�F�N�g�`��
void GameObjectRender();

//�S�I�u�W�F�N�g���
void GameObjectTerm();


//Beats�N���X�̃|�C���^��Ԃ�
Beats* GetBeats();