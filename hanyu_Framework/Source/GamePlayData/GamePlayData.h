#pragma once

//�Q�[���v���C���N���X
//�X�e�[�W�̔ԍ��E�N���A�ς݃t���O�̃A�N�Z�T
class  GamePlayData
{
public:
	void Init();
	int GetStageNo();
	void SetStageNo(int stageNo);

	bool IsStageCleared(int stageNo);
	void SetStageState(int stageNo, bool isCleared);
private:
	int mStageNo;

	enum
	{
		CLEAR_FLAG_ELEMENT_NUM = 4
	};
	unsigned char mClearFlags[CLEAR_FLAG_ELEMENT_NUM];
};


//�Q�[���v���C���̎擾
//GamePlayData& GetGamePlayData();
