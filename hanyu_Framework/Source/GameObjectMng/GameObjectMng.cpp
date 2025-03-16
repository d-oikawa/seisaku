#include "GameObjectMng.h"

//RnderManer���g���̂�include
#include "Fwk/Framework.h"

//��ʃT�C�Y�̒萔���g���̂�include
#include "AppDef.h"



Under_Circle mUnderCircle;
Upper_Circle mUpperCircle;
Beats mBeats;
NotesMng mNotesMng;

//�S�I�u�W�F�N�g������
//�eCSV�t�@�C������E���y�E�m�[�c�̏����擾����
void GameObjectInit()
{
	{
		CSVFile csvFile;
		csvFile.Load("Data/Musicdata.csv");
		mBeats.CreateMusic(csvFile.GetCSVData());
	}
	{
		CSVFile csvFile;
		csvFile.Load("Data/Notesdata.csv");
		mNotesMng.CreateNotes(csvFile.GetCSVData());
	}

	mBeats.Init();
	mUnderCircle.Init();
	mUpperCircle.Init();

	

}

//�S�I�u�W�F�N�g�X�V
void GameObjectUpdate()
{
	mUnderCircle.Update();
	mUpperCircle.Update();
	mBeats.Update();
}

//�S�I�u�W�F�N�g�`��
void GameObjectRender()
{
	mUnderCircle.Render();
	mUpperCircle.Render();
	mBeats.Render();
}

//�S�I�u�W�F�N�g���
void GameObjectTerm()
{
	mUnderCircle.Term();
	mUpperCircle.Term();
	mBeats.Term();
}


//Beats�N���X�̃|�C���^��Ԃ�
Beats* GetBeats()
{
	return &mBeats;
}