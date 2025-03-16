#include "Beats.h"
#include <Fwk/Framework.h>

//������
//���y�E�^�C�}�[�̏�����
//�P��������̕b���̌v�Z 1����/BPM(�S���q)=�P��������̒���*�P���߂�����̔���/�����q��(8���q�Ȃ�/2.0f or /(8.0f/4.0f))
void Beats::Init() {
		mSound.Load(musicFileName);
		mSoundSource.Init(mSound);
		mSoundSource.Play();
		mTimer = 0.0f;
		mBeatTimer = 0.0f;
		mBeats = 60.0f / (float)mBPM * 4.0f / 4.0f;
}

//�X�V
//�^�C�}�[�E���̍X�V
void Beats::Update() {
	mTimer += Time_I->GetDeltaTime();
	mBeatTimer += Time_I->GetDeltaTime();
	if (mBeats>mBeatTimer)
	{
		mBeatsCounts++;
		mBeatTimer = 0.0f;
	}
}

//�`��
//�Ȃ�
void Beats::Render() {

}

//�T�E���h�̊J��
void Beats::Term() {
		mSoundSource.Stop();
		mSoundSource.Term();
		mSound.Unload();
}

//CSV���特�y�f�[�^���擾����
//CSV�̏��́A�t�@�C�����A���O�A�ȑS�̂̒���(�b��)�ABPM
void Beats::CreateMusic(CSVData* pCsvData) {
	if (pCsvData == nullptr)
	{
		return;
	}

	int musicNum = pCsvData->GetElementCount() / 4;
	for (int i = 0; i < musicNum; i++)
	{
		int index = i * 4;
		pCsvData->GetString(index, &musicFileName);
		pCsvData->GetString(index + 1, &musicName);
		mSongLength = pCsvData->GetInt(index + 2);
		mBPM = pCsvData->GetInt(index + 3);
	}
	
}

//���݂��ȑS�̂ŉ����ڂ���Ԃ�
int Beats::GetBeats() {
	return mBeatsCounts;
}
//���݂̎��Ԃ�Ԃ�
float Beats::GetTimer() {
	return mTimer;
}

//�Ȃ��Đ�����Ă��邩�Ԃ�
//mSonglemgth���mTimer�̒l���傫���Ȃ�����true��Ԃ�
bool Beats::IsPlaying() {
	bool bEnd = false;
	if (mSongLength<mTimer)
	{
		bEnd = true;
	}
	return bEnd;
}

//�Ȃ̒�����Ԃ�
float Beats::GetSongLength() {
	return mSongLength;
}

//�Ȃ��~�߂�
void Beats::StopBeats() {
	mSoundSource.Stop();
}