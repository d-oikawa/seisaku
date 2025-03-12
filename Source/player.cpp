#include "player.h"

//�v���C���[�̏�����
//�e�N�X�`���E�T�C�Y�E�ʒu�E�T�E���h����ݒ肷��
void Player::Init() {

	mTexture.Load("Images/2dAction/player.png");
	mSprite.Init();
	mSprite.SetTexture(mTexture);
	mSprite.SetSize(64.0f, 64.0f);
	mSprite.SetPosition(mPosition);
}

//�v���C���\�̍X�V
//�ړ��E�Փ˔��蓙�̐ݒ�
void Player::Update() {

}

//�v���C���[�̕`��
void Player::Render() {

}

//�v���C���[�̌�Еt��
void Player::Term() {
	mTexture.Unload();
	mSprite.Term();
}

//���݈ʒu���擾����
Vector2f Player::GetPosition() {
	return mPosition;
}