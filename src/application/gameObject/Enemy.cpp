#include "Enemy.h"
#include "mathOriginal.h"
#include "Vector3.h"
#include "Imgui.h"

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

	//���S�`�F�b�N
	//CheckIsDead();

	//����
	TypeUpdate();

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
		if (!isDead_[i]) {

			objects->SetPosition(position[i]);
			objects->SetRotation(rotation[i]);
			objects->SetScale(scale[i]);
		}

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

void Enemy::Draw(ID3D12GraphicsCommandList* cmdList)
{
	bullet->Draw(cmdList);
}

void Enemy::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	bullet->DrawLightView(cmdList);
}

void Enemy::SetSRV(ID3D12DescriptorHeap* SRV)
{
	bullet->SetSRV(SRV);
}

void Enemy::TypeUpdate()
{

	//�ˌ��X�V
	if (shotCoolTimer > ShotCoolTime) {
		isShot = true;
	}
	else {
		shotCoolTimer++;
	}

	int i = 0;
	for (std::unique_ptr<FbxObject3D>& objects : object) {

		if (!isDead_[i]) {
			switch (type_[i]) {

				//�ړ��n
			case MoveXEnemy:
				MoveX(i);
				break;

			case MoveZEnemy:
				MoveZ(i);
				break;

			case MoveXZEnemy:
				MoveX(i);
				MoveZ(i);
				break;

			case HomingMoveEnemy:
				MoveHoming(i);
				break;

				//�ˌ��n
			case NormalShotXEnemy:
				ShotX(i);
				break;

			case NormalShotZEnemy:
				ShotZ(i);
				break;

			case NormalShotXZEnemy:
				ShotX(i);
				ShotZ(i);
				break;

			case HomingShotEnemy:
				ShotHoming(i);
				break;

				//�����n,����n
			case HomingMoveShotEnemy:
				MoveHoming(i);
				ShotHoming(i);
				break;
			}
		}

		i++;
	}

	//�ˌ��X�V
	bullet->Update();

	if (isShot) {
		isShot = false;
		shotCoolTimer = 0;
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

void Enemy::Shot(int i, XMFLOAT3 velosity)
{
	////�V���b�g�t���O�𗧂Ă�
	//bullet->SetShotFlag(true);
	//�e�����ꏊ��velocity���Z�b�g
	bullet->SetBullet(position[i], velosity);
	//�e�̐���
	bullet->CreateBullet();
}

void Enemy::ShotX(int i)
{
	if (isShot) {

		//---�E�����Ɏˌ�---

		XMFLOAT3 rightVec = { bulletSpeed,0,0 };
		Shot(i, rightVec);

		//---�������Ɏˌ�---

		XMFLOAT3 leftVec = { -bulletSpeed,0,0 };
		Shot(i, leftVec);
	}
}

void Enemy::ShotZ(int i)
{
	if (isShot) {

		//---������Ɏˌ�---

		XMFLOAT3 upVec = { 0,0,bulletSpeed };
		Shot(i, upVec);

		//---�������Ɏˌ�---

		XMFLOAT3 downVec = { 0,0,-bulletSpeed };
		Shot(i, downVec);
	}
}

void Enemy::ShotHoming(int i)
{

	if (isShot) {

		//���@�ƓG�̃x�N�g����p��
		Vector3 playerVec = { playerPosition.x,playerPosition.y,playerPosition.z };
		Vector3 enemyVec = { position[i].x,position[i].y,position[i].z };

		//���@�Ƃ̃x�N�g�������
		Vector3 velocity = playerVec - enemyVec;

		//���K�������đ��x��������
		velocity.normalize();
		velocity *= bulletSpeed;

		XMFLOAT3 shotVec = { velocity.x,0,velocity.z };
		Shot(i, shotVec);
	}
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
	enemyNum++;

	if (object->GetFileName() == "enemy_homingShotEnemy")SetTypeData(HomingShotEnemy); 
	else if (object->GetFileName() == "enemy_normalShotXEnemy")SetTypeData(NormalShotXEnemy);
	else if (object->GetFileName() == "enemy_normalShotZEnemy")SetTypeData(NormalShotZEnemy);
	else if (object->GetFileName() == "enemy_moveXZEnemy")SetTypeData(MoveXZEnemy);
	else if (object->GetFileName() == "enemy_homingMoveShotEnemy")SetTypeData(HomingMoveShotEnemy);
	else if (object->GetFileName() == "enemy_homingMoveEnemy")SetTypeData(HomingMoveEnemy);
	else if (object->GetFileName() == "enemy_normalShotXZEnemy")SetTypeData(NormalShotXZEnemy);
	else if (object->GetFileName() == "enemy_normalShotZEnemy")SetTypeData(NormalShotZEnemy);
	else { SetTypeData(HomingShotEnemy); }

	SetTypeData(HomingMoveShotEnemy);
	/*SetStageNumber();*/
}

void Enemy::HitPlane()
{
	////�ڒn�t���O�𗧂Ă�
	//groundFlag = true;

	////�߂荞�܂Ȃ��Ȃ�܂ŉ��Z
	//position.y += 0.1f;
}

void Enemy::SetTypeData(int type)
{
	//���ʕ����f�[�^�Z�b�g
	this->type_.push_back(type);
	moveX.push_back(true);
	moveZ.push_back(true);
	isDead_.push_back(false);

	//�^�C�v���Ƃ̏��Z�b�g
	int hp = 0;

	switch (type) {

		//�ړ��n
	case MoveXEnemy:
		hp = 1;
		break;

	case MoveZEnemy:
		hp = 1;
		break;

	case MoveXZEnemy:
		hp = 1;
		break;

	case HomingMoveEnemy:
		hp = 1;
		break;

		//�ˌ��n
	case NormalShotXEnemy:
		hp = 3;
		break;

	case NormalShotZEnemy:
		hp = 3;
		break;

	case NormalShotXZEnemy:
		hp = 3;
		break;

	case HomingShotEnemy:
		hp = 3;
		break;

		//�����n,����n
	case HomingMoveShotEnemy:
		hp = 1;
		break;
	}

	hp_.push_back(hp);

}

void Enemy::SetStageNumber()
{
	int i = 0;
	for (std::unique_ptr<FbxObject3D>& objects : object)
	{
		if (objects->GetFileName() == "enemy1_homingShotEnemy" || objects->GetFileName() == "enemy1_normalShotXEnemy"
			|| objects->GetFileName() == "enemy1_normalShotYEnemy" || objects->GetFileName() == "enemy1_normalShotZEnemy")
		{
			stageNember_.emplace_back(1);
		}
		if (objects->GetFileName() == "enemy2_homingShotEnemy" || objects->GetFileName() == "enemy2_normalShotXEnemy"
			|| objects->GetFileName() == "enemy2_normalShotYEnemy" || objects->GetFileName() == "enemy2_normalShotZEnemy")
		{
			stageNember_.emplace_back(2);
		}
		if (objects->GetFileName() == "enemy3_homingShotEnemy" || objects->GetFileName() == "enemy3_normalShotXEnemy"
			|| objects->GetFileName() == "enemy3_normalShotYEnemy" || objects->GetFileName() == "enemy3_normalShotZEnemy")
		{
			stageNember_.emplace_back(3);
		}
		if (objects->GetFileName() == "enemy4_homingShotEnemy" || objects->GetFileName() == "enemy2_normalShotXEnemy"
			|| objects->GetFileName() == "enemy4_normalShotYEnemy" || objects->GetFileName() == "enemy2_normalShotZEnemy")
		{
			stageNember_.emplace_back(4);
		}
		if (objects->GetFileName() == "enemy5_homingShotEnemy" || objects->GetFileName() == "enemy2_normalShotXEnemy"
			|| objects->GetFileName() == "enemy5_normalShotYEnemy" || objects->GetFileName() == "enemy2_normalShotZEnemy")
		{
			stageNember_.emplace_back(5);
		}
		if (objects->GetFileName() == "enemy6_homingShotEnemy" || objects->GetFileName() == "enemy2_normalShotXEnemy"
			|| objects->GetFileName() == "enemy6_normalShotYEnemy" || objects->GetFileName() == "enemy2_normalShotZEnemy")
		{
			stageNember_.emplace_back(6);
		}
		if (objects->GetFileName() == "enemy7_homingShotEnemy" || objects->GetFileName() == "enemy2_normalShotXEnemy"
			|| objects->GetFileName() == "enemy7_normalShotYEnemy" || objects->GetFileName() == "enemy2_normalShotZEnemy")
		{
			stageNember_.emplace_back(7);
		}
		if (objects->GetFileName() == "enemy8_homingShotEnemy" || objects->GetFileName() == "enemy2_normalShotXEnemy"
			|| objects->GetFileName() == "enemy8_normalShotYEnemy" || objects->GetFileName() == "enemy2_normalShotZEnemy")
		{
			stageNember_.emplace_back(8);
		}
		if (objects->GetFileName() == "enemy9_homingShotEnemy" || objects->GetFileName() == "enemy2_normalShotXEnemy"
			|| objects->GetFileName() == "enemy9_normalShotYEnemy" || objects->GetFileName() == "enemy2_normalShotZEnemy")
		{
			stageNember_.emplace_back(9);
		}
		i++;
	}
}


JSONLoader::ColliderData Enemy::GetColliderData(int num)
{
	std::list<std::unique_ptr<FbxObject3D>>::iterator itr;
	itr = object.begin();
	if (num != 0)
	{
		for (int i = 0; i < num; i++)
		{
			itr++;
		}
	}

	return itr->get()->GetColliderData();
}

void Enemy::OnCollisionToEnemy(int i, XMFLOAT3 enemyPos)
{

	switch (type_[i]) {

		//�ړ��n
	case MoveXEnemy:
		RefMoveX(i);
		break;

	case MoveZEnemy:
		RefMoveZ(i);
		break;

	case MoveXZEnemy:
		RefMoveX(i);
		RefMoveZ(i);
		break;

	case HomingMoveEnemy:
		RefVec(i, enemyPos);
		break;

		//�ˌ��n
	case NormalShotXEnemy:
	
		break;

	case NormalShotZEnemy:

		break;

	case NormalShotXZEnemy:

		break;

	case HomingShotEnemy:
	
		break;

		//�����n,����n
	case HomingMoveShotEnemy:
		RefVec(i,enemyPos);
		break;
	}

}

void Enemy::OnCollisionToPlayer(int i,XMFLOAT3 playerPos)
{
	//�G���m�Ɠ�������
	OnCollisionToEnemy(i,playerPos);
}

void Enemy::OnCollisionToBullet(int i)
{
	//hp��
	hp_[i]--;

	//hp��0�ȉ��Ȃ玀�S
	if (hp_[i] <= 0) {
		isDead_[i] = true;
	}
}

void Enemy::CheckIsDead()
{
	for (int i = 0; i < object.size(); i++) {

		//���S���Ă�����v�f��z�񂩂�폜
		if (isDead_[i]) {
			position.erase(position.begin() + i);
			rotation.erase(rotation.begin() + i);
			scale.erase(scale.begin() + i);
			type_.erase(type_.begin() + i);
			hp_.erase(hp_.begin() + i);
			isDead_.erase(isDead_.begin() + i);
			moveX.erase(moveX.begin() + i);
			moveZ.erase(moveZ.begin() + i);
			object.erase(std::next(object.begin(), i));
		}
	}
}

void Enemy::RefVec(int i, XMFLOAT3 enemyPos)
{
	//�G���m�̃x�N�g����p��
	Vector3 enemy1 = { enemyPos.x,enemyPos.y,enemyPos.z };
	Vector3 enemy2 = { position[i].x,position[i].y,position[i].z };

	//�G���m�̃x�N�g�������
	Vector3 velocity = enemy1 - enemy2;

	//���K�������đ��x��������
	velocity.normalize();
	velocity *= enemySpeed;

	//�ړ��x�N�g�������Z
	position[i].x += -velocity.x;
	position[i].z += -velocity.z;
}

void Enemy::RefMoveX(int i)
{
	if (moveX[i]) {
		moveX[i] = 0;
	}
	else {
		moveX[i] = 1;
	}
}

void Enemy::RefMoveZ(int i)
{
	if (moveZ[i]) {
		moveZ[i] = 0;
	}
	else {
		moveZ[i] = 1;
	}
}

void Enemy::DeleteEnemy()
{
	/*if*/
}

