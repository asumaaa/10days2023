#include "Enemy.h"
#include "mathOriginal.h"
#include "Vector3.h"

#define G 6.674	//���L���͒萔
#define GAcceleration 9.80665 * 1/10	//�d�͉����x

Camera* Enemy::camera = nullptr;
Input* Enemy::input = nullptr;
DXInput* Enemy::dxInput = nullptr;

void Enemy::Initialize()
{
	Sprite* newSprite = new Sprite();
	newSprite->SetTextureNum(4);
	newSprite->Initialize();
	spriteHpBar.reset(newSprite);
}

void Enemy::Update(XMFLOAT3 playerPos)
{
	//�v���C���[���W�擾
	this->playerPosition = playerPos;

	//����
	Move();

	//�I�u�W�F�N�g�X�V
	UpdateObject();

	//�X�v���C�g�X�V
	UpdateSprite();
}

void Enemy::UpdateObject()
{
	//�X�V
	int i = 0;
	for (std::unique_ptr<FbxObject3D>& objects : object)
	{
		objects->SetPosition(position[i]);
		objects->SetRotation(rotation[i]);
		objects->SetScale(scale[i]);
		objects->Update();
		i++;
	}

	/*object->Update();*/
}

void Enemy::UpdateSprite()
{
	//HP�o�[
	spriteHpBar->SetAlpha(1.0f);
	spriteHpBar->SetScale({ 100.0f, 100.0 });
	spriteHpBar->SetPosition({ 0.0f, 0.0 });
	spriteHpBar->Update();
}

void Enemy::DrawSprite(ID3D12GraphicsCommandList* cmdList)
{
	spriteHpBar->Draw(cmdList);
}

void Enemy::Move()
{
	int i = 0;
	for (std::unique_ptr<FbxObject3D>& objects : object) {

		switch (type[i]) {

		case HomingMoveShotEnemy:
			break;

		case HomingMoveEnemy:
			MoveHoming(i);
			break;

		case NormalShotEnemy:
			break;

		case HomingShotEnemy:
			break;

		case MoveXEnemy:
			MoveX(i);
			break;

		case MoveZEnemy:
			MoveZ(i);
			break;
		}

		i++;
	}

}

void Enemy::MoveHoming(int i)
{
	//���@�ƓG�̃x�N�g����p��
	Vector3 playerVec = { playerPosition.x,playerPosition.y,playerPosition.z };
	Vector3 enemyVec = { position[i].x,position[i].y,position[i].z };

	//���@�Ƃ̃x�N�g�������
	Vector3 velocity = playerVec - enemyVec;

	//���K�������đ��x��������
	velocity.normalize();
	velocity *= enemySpeed;

	//�ړ��x�N�g�������Z
	position[i].x += velocity.x;
	position[i].z += velocity.z;

}

void Enemy::MoveX(int i)
{
	//�X�e�[�W�̒[���v�Z
	float stageRight = stageMid.x + (stageSize.x);
	float stageLeft = stageMid.x - (stageSize.x);

	//�X�e�[�W�[�ɗ����甽�˂�����
	if (moveX[i]) {
		if (position[i].x > stageRight) {
			moveX[i] = false;
		}
	}
	else {
		if (position[i].x < stageLeft) {
			moveX[i] = true;
		}
	}

	//�ړ�
	if (moveX[i]) {
		//�E�Ɉړ�
		position[i].x += enemySpeed;
	}
	else {
		//���Ɉړ�
		position[i].x -= enemySpeed;
	}
}

void Enemy::MoveZ(int i)
{
	//�X�e�[�W�̒[���v�Z
	float stageUp = stageMid.z + (stageSize.z);
	float stageDown = stageMid.z - (stageSize.z);

	//�X�e�[�W�[�ɗ����甽�˂�����
	if (moveZ[i]) {
		if (position[i].z > stageUp) {
			moveZ[i] = false;
		}
	}
	else {
		if (position[i].z < stageDown) {
			moveZ[i] = true;
		}
	}

	//�ړ�
	if (moveZ[i]) {
		//�E�Ɉړ�
		position[i].z += enemySpeed;
	}
	else {
		//���Ɉړ�
		position[i].z -= enemySpeed;
	}
}

void Enemy::Shot()
{
}

void Enemy::UpdateGravity()
{
	////�ڒn���Ă�����^�C�}�[�ƃx�N�g�����Z�b�g
	//if (groundFlag == true)
	//{
	//	fallTimer = 0.0f;
	//	fallVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//}

	////�ڒn���Ă��Ȃ����
	//if (groundFlag == false)
	//{
	//	//�����^�C�}�[���ő�l��菬�������
	//	if (fallTimer < fallTime)
	//	{
	//		fallTimer += fallFrame;
	//	}
	//}

	////�����x�N�g���v�Z
	//fallVelocity.y = -(GAcceleration * fallTimer);

	////���W�ɗ����x�N�g�������Z
	//position = position + fallVelocity;

	//���W�ɗ����x�N�g�������Z
	/*position = position + fallVelocity;*/
}

void Enemy::UpdateJump()
{
	//�ڒn���Ă�����
	if (groundFlag == true)
	{
		//�X�y�[�X�L�[�ŃW�����v
		if (input->TriggerKey(DIK_SPACE))
		{
			groundFlag = false;
			fallTimer = -jumpHeight;
			fallVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
		}
	}
}

void Enemy::UpdateAttack()
{
}

void Enemy::SetObject(FbxObject3D* object)
{
	//�����̃I�u�W�F�N�g���Z�b�g
	Enemy::object.emplace_back(object);

	position.emplace_back(object->GetPosition());
	rotation.emplace_back(object->GetRotation());
	scale.emplace_back(object->GetScale());

	type.push_back(HomingMoveEnemy);
	moveX.push_back(true);
	moveZ.push_back(true);
}

void Enemy::HitPlane()
{
	////�ڒn�t���O�𗧂Ă�
	//groundFlag = true;

	////�߂荞�܂Ȃ��Ȃ�܂ŉ��Z
	//position.y += 0.1f;
}