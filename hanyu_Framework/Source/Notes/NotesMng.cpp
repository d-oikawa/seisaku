#include "NotesMng.h"

//SAFE_DELETE���g���ׂɃC���N���[�h
#include "Lib/LibDef.h"

//��������m�[�c�̃N���X�̐錾
#include "Under_Notes.h"
#include "Upper_Notes.h"

//������
void NotesMng::Init() {
	//�A�N�e�B�u�ȃm�[�c�̃|�C���^�z�����ɂ��Ă���
	mActiveNotes.clear();
}

//��Еt��
void NotesMng::Term() {
	//�A�N�e�B�u�ȃm�[�c�̃|�C���^�̔z�����ɂ��Ă���
	mActiveNotes.clear();
	//�S�Ẵm�[�c�v�[����j��
	DestroyPoolALL();
}

//�X�V
void NotesMng::Update() {
	//�A�N�e�B�u�ȃm�[�c�����ׂčX�V����
	for (int i = 0; i < mActiveNotes.size(); i++)
	{
		//�X�V
		mActiveNotes[i]->Update();
	}

	//�X�V�̌��ʔ�A�N�e�B�u�ƂȂ����v�f����菜��
	for (int i = 0; i < mActiveNotes.size();)
	{
		if (mActiveNotes[i]->IsActive())
		{
			//mActiveEnemies[i]���A�N�e�B�u�̂܂܂ł����
			//���̗v�f����������̂�i���C���N�������g
			++i;
		}
		else
		{
			//mActiveEnemies[i]����A�N�e�B�u�ƂȂ��Ă����炻�̗v�f����菜��
			mActiveNotes.erase(mActiveNotes.begin() + i);
		}
	}
}

//�`��
void NotesMng::Render() {
	//�A�N�e�B�u�ȃm�[�c�����ׂĕ`�悷��
	for (int i = 0; i < mActiveNotes.size(); i++)
	{
		//�X�V
		mActiveNotes[i]->Render();
	}
}

//�I�u�W�F�N�g�v�[���̐���
void NotesMng::GeneratePool(string notesName, int poolSize) {
	//���m�[�c�̃v�[���̐���
	if (notesName == "Under_Notes")
	{
		//��������m�[�c�N���X���w��
		mNotesPools[notesName].Init<Under_Notes>(poolSize);
	}
	//��m�[�c�̃v�[���̐���
	if (notesName == "Upper_Notes")
	{
		//��������m�[�c�N���X���w��
		mNotesPools[notesName].Init<Upper_Notes>(poolSize);
	}

}

//�w�肵���m�[�c�̃I�u�W�F�N�g�v�[���̔j��
void NotesMng::DestroyPool(string notesName) {
	//�m�[�c�̃v�[���j��
	if (mNotesPools.count(notesName) > 0)
	{
		mNotesPools[notesName].Term();
		mNotesPools.erase(notesName);
	}
}

//�S�Ẵm�[�c�̃I�u�W�F�N�g�v�[���̔j��
void NotesMng::DestroyPoolALL() {
	//���m�[�c�̃o�b�t�@��j��
	DestroyPool("Under_Notes");
	//��m�[�c�̃o�b�t�@��j��
	DestroyPool("Upper_Notes");
}

//�m�[�c�̐���
void NotesMng::CreateNotes(string notesName, float timer) {
	//�w��̃m�[�c�v�[�������A�N�e�B�u�̃I�u�W�F�N�g���擾����
	Notes* pNotes = nullptr;

	//�m�[�c�v�[������󂫃I�u�W�F�N�g���擾
	if (mNotesPools.count(notesName) > 0)
	{
		pNotes = mNotesPools[notesName].Alloc();
	}

	//��A�N�e�B�u�̃I�u�W�F�N�g���Ȃ���ΐ������s
	if (pNotes == nullptr)
	{
		return;
	}
	//���Ԃ�ݒ��ݒ�
	pNotes->SetTimer(timer);

	//�������Ɋ֐����Ăяo��
	pNotes->OnCreated();

	//�A�N�e�B�u�ȃm�[�c�z��ɒǉ����Ă���
	mActiveNotes.push_back(pNotes);
}

//CSV�f�[�^����m�[�c�𐶐�����
void NotesMng::CreateNotes(CSVData* pCsvData) {
	//nullptr���n����Ă����牽�����Ȃ��Ŋ֐��𔲂���
	if (pCsvData == nullptr)
	{
		return;
	}

	//�m�[�c�̏��́A���O�A�\������b����2�łP�g�B
	//CSV�t�@�C���̑S�v�f��2�Ŋ����ăm�[�c�̔z�u���̐��Ƃ���
	int notesNum = pCsvData->GetElementCount() / 2;
	//�z�u����m�[�c�̐������J��Ԃ�
	for (int i = 0; i < notesNum; i++)
	{
		//�m�[�c�P�̂̔z�u�f�[�^�̐擪�̗v�f�ԍ�
		int index = i * 2;
		//�m�[�c�̖��O���擾
		string notesName;
		pCsvData->GetString(index, &notesName);
		//�z�u��̍��W���擾
		float timer;
		timer = pCsvData->GetFloat(index + 1);

		//�m�[�c�𐶐�
		CreateNotes(notesName, timer);
	}
}

//�c��̃m�[�c�̐�
int NotesMng::GetNotesCount() {
	return (int)mActiveNotes.size();
}
