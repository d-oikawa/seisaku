#include "NotesPool.h"

//SAFE_DELETE���g�������̂�#include
#include "Fwk/Framework.h"

//��Еt��
void NotesPool::Term() {
	//Notes*�z��̊e�v�f��new���ꂽ�G�̃I�u�W�F�N�g�̃A�h���X��
	//�L�^����Ă���̂ŁA1�P��DELETE���Ă���
	for (int i = 0; i < mPool.size(); i++)
	{
		SAFE_DELETE(mPool[i]);
	}
	//�v�[���j��
	mPool.clear();
}

//�I�u�W�F�N�g���P���������Ė߂�
Notes* NotesPool::Alloc() {
	//�߂�l���L�^����ϐ�
	Notes* pResult = nullptr;

	for (int i = 0; i < mPool.size(); i++)
	{
		//��A�N�e�B�u�̃I�u�W�F�N�g���������犈�������Ė߂�l�ɂ���B
		if (mPool[i]->IsActive() == false)
		{
			//�߂�l�ɂ���
			pResult = mPool[i];
			//���������Ă���
			pResult->SetActive(true);
			//���[�v�͔����Ă悢
			break;
		}
	}
	return pResult;
}