#pragma once

#include <string>
#include <vector>
#include <map>
using namespace std;

#include "Lib/Math/Vector.h"
using namespace Lib::Math;

//CSV�f�[�^���g���̂�CSVFile.h����荞��
#include "Fwk/File/CSVFile.h"

//��������m�[�c�̃N���X�̐錾����荞��
#include "Notes.h"

//�m�[�c�v�[���N���X���g���̂�
#include "NotesPool.h"

//�m�[�c�Ǘ��N���X
class  NotesMng
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

	//�I�u�W�F�N�g�v�[���̐���
	void GeneratePool(string notesName, int poolSize);
	//�w�肵���m�[�c�̃I�u�W�F�N�g�v�[���̊J��
	void DestroyPool(string notesName);
	//���ׂẴm�[�c�̃I�u�W�F�N�g�v�[���̊J��
	void DestroyPoolALL();

	//�m�[�c�̐���
	void CreateNotes(string notesName, float timer);

	//�c��̃m�[�c�̐�
	int GetNotesCount();

	//CSV�f�[�^����m�[�c�𐶐�����
	void CreateNotes(CSVData* pCsvData);

private:
	//�A�N�e�B�u�ȃm�[�c�I�u�W�F�N�g�̃|�C���^�z��
	vector<Notes*> mActiveNotes;
	//�m�[�c��ʂ��Ƃ̃I�u�W�F�N�g�v�[��
	map<string, NotesPool>mNotesPools;
};

