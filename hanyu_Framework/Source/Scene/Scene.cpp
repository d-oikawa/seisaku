#include "Scene.h"

#include "Fwk/Framework.h"

//������
void Scene::Init() {
	//���̃V�[���̏����l���Ȃ��ɂ��Ă���
	mNextScene = SceneType::None;
}

//�I��
void Scene::Term() {
	//�Ȃ�
}

//�X�V
void Scene::Update() {
	//�Ȃ�
}

//�`��
void Scene::Render() {
	//�Ȃ�
}

//���̃V�[�����擾����
SceneType Scene::GetNextScene() {
	return mNextScene;
}