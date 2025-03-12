#include "GamePlayData.h"

GamePlayData playData;

//�Q�[���v���C���̎擾
GamePlayData& GetGamePlayerData() {
	return playData;
}

//������
void GamePlayData::Init() {
	mStageNo = 0;
	for (int i = 0; i < CLEAR_FLAG_ELEMENT_NUM; i++)
	{
		mClearFlags[i] = 0;
	}
}

//�X�e�[�W�ԍ��̃A�N�Z�T
int GamePlayData::GetStageNo() {
	return mStageNo;
}

void GamePlayData::SetStageNo(int stageNo) {
	mStageNo = stageNo;
}

//�N���A�ς݂̃t���O�A�N�Z�T
bool GamePlayData::IsStageCleared(int stageNo) {
	int elementIndex = stageNo / 8;
	int flagNo = stageNo % 8;
	if (elementIndex < 0 || elementIndex >= CLEAR_FLAG_ELEMENT_NUM)
	{
		return false;
	}
	return mClearFlags[elementIndex] & (1 << flagNo);
}

//�t���O�̊Ǘ�
//true�ŗ��Ă�Afalse�ŉ�����
void GamePlayData::SetStageState(int stageNo, bool isCleared) {
	int elementIndex = stageNo / 8;
	int flagNo = stageNo % 8;
	if (elementIndex < 0 || elementIndex >= CLEAR_FLAG_ELEMENT_NUM)
	{
		return ;
	}
	if (isCleared)
	{
		
		mClearFlags[elementIndex] |= (1 << flagNo);
	}
	else
	{
		mClearFlags[elementIndex] |= ~(1 << flagNo);
	}
}