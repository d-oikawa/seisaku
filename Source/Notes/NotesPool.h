#pragma once

//�G�v�[���N���X

//�ϕ\�L�z��std::vector���g��
#include <vector>

#include "Notes.h"

class NotesPool
{
public:
	//������
	//class T�́AInit�֐��̌Ăяo�����ɋ�̓I�ȓG�N���X�ɒu�������B
	template<class T>
	void Init(int poolSize) {
		//�v�[���S�v�f���̓G�I�u�W�F�N�g�𐶐�
		for (int i = 0; i < poolSize; ++i) {
			//�w�肳�ꂽ�G�N���X�ϐ���1����
			//Notes+�ɃL���X�g�ł���N���X���w�肳���̂��O��
			Notes* pNewNotes = new T();
			//������
			pNewNotes->Init();
			//�ŏ��͔�A�N�e�B�u
			pNewNotes->SetActive(false);
			//�v�[���ɃA�h���X���L�^���Ă���
			mPool.push_back(pNewNotes);
		}
	}
	//��Еt��
	void Term();
	//�I�u�W�F�N�g���P���������Ė߂�
	Notes* Alloc();

protected:
	//Notes*�^�̉ϒ��z��
	std::vector<Notes*>mPool;
};