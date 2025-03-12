#pragma once

//�V�[����ʂ̗񋓌^���g���̂�include
#include "GameDef.h"

//�V�[���N���X
class Scene
{
public:
	//������
	virtual void Init();
	//��Еt��
	virtual void Term();
	//�X�V
	virtual void Update();
	//�`��
	virtual void Render();
	//���̃V�[�����擾����
	SceneType GetNextScene();
protected:
	//���̃V�[��
	SceneType mNextScene;

};