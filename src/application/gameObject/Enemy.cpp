#include "Enemy.h"
#include "mathOriginal.h"
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

void Enemy::Update()
{
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
	////�W�����v�X�V
	//UpdateJump();

	//�d�͍X�V
	/*UpdateGravity();*/

	//position.x -= input->PushKey(DIK_A) * speed;
	//position.x += input->PushKey(DIK_D) * speed;
	//position.z -= input->PushKey(DIK_S) * speed;
	//position.z += input->PushKey(DIK_W) * speed;
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
}

void Enemy::HitPlane()
{
	////�ڒn�t���O�𗧂Ă�
	//groundFlag = true;

	////�߂荞�܂Ȃ��Ȃ�܂ŉ��Z
	//position.y += 0.1f;
}