#include "Enemy.h"
#include "mathOriginal.h"
#include "Vector3.h"
#include "Imgui.h"

#define G 6.674	//万有引力定数
#define GAcceleration 9.80665 * 1/10	//重力加速度

Camera* Enemy::camera = nullptr;
Input* Enemy::input = nullptr;
DXInput* Enemy::dxInput = nullptr;

void Enemy::Initialize()
{
	Sprite* newSprite = new Sprite();
	newSprite->SetTextureNum(4);
	newSprite->Initialize();
	spriteHpBar.reset(newSprite);

	for (int i = 0; i < 9; i++)
	{
		stageFlag[i] = false;
		stageClearFlag[i] = false;
		stageEnemyNum[i] = 0;
	}
}

void Enemy::Update(XMFLOAT3 playerPos)
{
	//プレイヤー座標取得
	this->playerPosition = playerPos;

	//死亡チェック
	//CheckIsDead();

	//動き
	TypeUpdate();

	CheckDeadEnemy();

	//オブジェクト更新
	UpdateObject();

	//スプライト更新
	UpdateSprite();

}

void Enemy::UpdateObject()
{
	//更新
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
	//HPバー
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

	//射撃更新
	if (shotCoolTimer > ShotCoolTime) {
		isShot = true;
	}
	else {
		shotCoolTimer++;
	}

	int i = 0;
	for (std::unique_ptr<FbxObject3D>& objects : object) {
		if (stageNum == stageNember_[i])
		{
			if (!isDead_[i]) {
				switch (type_[i]) {

					//移動系
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

					//射撃系
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

					//複合系
				case HomingMoveShotEnemy:
					MoveHoming(i);
					ShotHoming(i);
					break;
				}
			}
		}

		i++;
	}

	//射撃更新
	bullet->Update();

	if (isShot) {
		isShot = false;
		shotCoolTimer = 0;
	}

}

void Enemy::MoveHoming(int i)
{
	//自機と敵のベクトルを用意
	Vector3 playerVec = { playerPosition.x,playerPosition.y,playerPosition.z };
	Vector3 enemyVec = { position[i].x,position[i].y,position[i].z };

	//自機とのベクトルを取る
	Vector3 velocity = playerVec - enemyVec;

	//正規化をして速度をかける
	velocity.normalize();
	velocity *= enemySpeed;

	//移動ベクトルを加算
	position[i].x += velocity.x;
	position[i].z += velocity.z;

}

void Enemy::MoveX(int i)
{
	//ステージの端を計算
	float stageRight = stageMid.x + (stageSize.x);
	float stageLeft = stageMid.x - (stageSize.x);

	//ステージ端に来たら反射させる
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

	//移動
	if (moveX[i]) {
		//右に移動
		position[i].x += enemySpeed;
	}
	else {
		//左に移動
		position[i].x -= enemySpeed;
	}
}

void Enemy::MoveZ(int i)
{
	//ステージの端を計算
	float stageUp = stageMid.z + (stageSize.z);
	float stageDown = stageMid.z - (stageSize.z);

	//ステージ端に来たら反射させる
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

	//移動
	if (moveZ[i]) {
		//右に移動
		position[i].z += enemySpeed;
	}
	else {
		//左に移動
		position[i].z -= enemySpeed;
	}
}

void Enemy::Shot(int i, XMFLOAT3 velosity)
{
	////ショットフラグを立てる
	//bullet->SetShotFlag(true);
	//弾生成場所とvelocityをセット
	bullet->SetBullet(position[i], velosity);
	//弾の生成
	bullet->CreateBullet();
}

void Enemy::ShotX(int i)
{
	if (isShot) {

		//---右方向に射撃---

		XMFLOAT3 rightVec = { bulletSpeed,0,0 };
		Shot(i, rightVec);

		//---左方向に射撃---

		XMFLOAT3 leftVec = { -bulletSpeed,0,0 };
		Shot(i, leftVec);
	}
}

void Enemy::ShotZ(int i)
{
	if (isShot) {

		//---上方向に射撃---

		XMFLOAT3 upVec = { 0,0,bulletSpeed };
		Shot(i, upVec);

		//---下方向に射撃---

		XMFLOAT3 downVec = { 0,0,-bulletSpeed };
		Shot(i, downVec);
	}
}

void Enemy::ShotHoming(int i)
{

	if (isShot) {

		//自機と敵のベクトルを用意
		Vector3 playerVec = { playerPosition.x,playerPosition.y,playerPosition.z };
		Vector3 enemyVec = { position[i].x,position[i].y,position[i].z };

		//自機とのベクトルを取る
		Vector3 velocity = playerVec - enemyVec;

		//正規化をして速度をかける
		velocity.normalize();
		velocity *= bulletSpeed;

		XMFLOAT3 shotVec = { velocity.x,0,velocity.z };
		Shot(i, shotVec);
	}
}

void Enemy::UpdateGravity()
{
	////接地していたらタイマーとベクトルリセット
	//if (groundFlag == true)
	//{
	//	fallTimer = 0.0f;
	//	fallVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//}

	////接地していなければ
	//if (groundFlag == false)
	//{
	//	//落下タイマーが最大値より小さければ
	//	if (fallTimer < fallTime)
	//	{
	//		fallTimer += fallFrame;
	//	}
	//}

	////落下ベクトル計算
	//fallVelocity.y = -(GAcceleration * fallTimer);

	////座標に落下ベクトルを加算
	//position = position + fallVelocity;

	//座標に落下ベクトルを加算
	/*position = position + fallVelocity;*/
}

void Enemy::UpdateAttack()
{
}

void Enemy::SetObject(FbxObject3D* object)
{
	//引数のオブジェクトをセット
	Enemy::object.emplace_back(object);

	position.emplace_back(object->GetPosition());
	rotation.emplace_back(object->GetRotation());
	scale.emplace_back(object->GetScale());
	enemyNum++;

	if (object->GetFileName() == "enemy_moveXEnemy")SetTypeData(MoveXEnemy);
	else if (object->GetFileName() == "enemy_moveZEnemy")SetTypeData(MoveZEnemy);
	else if (object->GetFileName() == "enemy_moveXZEnemy")SetTypeData(MoveXZEnemy);
	else if (object->GetFileName() == "enemy_homingMoveEnemy")SetTypeData(HomingMoveEnemy);

	else if (object->GetFileName() == "enemy_normalShotXEnemy")SetTypeData(NormalShotXEnemy);
	else if (object->GetFileName() == "enemy_normalShotZEnemy")SetTypeData(NormalShotZEnemy);
	else if (object->GetFileName() == "enemy_normalShotXZEnemy")SetTypeData(NormalShotXZEnemy);
	else if (object->GetFileName() == "enemy_homingShotEnemy")SetTypeData(HomingShotEnemy);
	else  if (object->GetFileName() == "enemy_homingMoveShotEnemy")SetTypeData(HomingMoveShotEnemy);

	//SetTypeData(HomingMoveShotEnemy);
	SetStageNumber(object->GetPosition());
}

void Enemy::HitPlane()
{
	////接地フラグを立てる
	//groundFlag = true;

	////めり込まなくなるまで加算
	//position.y += 0.1f;
}

void Enemy::SetTypeData(int type)
{
	//共通部分データセット
	this->type_.push_back(type);
	moveX.push_back(true);
	moveZ.push_back(true);
	isDead_.push_back(false);

	//タイプごとの情報セット
	int hp = 0;

	switch (type) {

		//移動系
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

		//射撃系
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

		//複合系,特殊系
	case HomingMoveShotEnemy:
		hp = 1;
		break;
	}

	hp_.push_back(hp);

}

void Enemy::SetStageNumber(XMFLOAT3 pos)
{
	if (pos.x >= -40 && pos.x < 40)
	{
		if (pos.z >= -40 && pos.z < 40)
		{
			stageNember_.emplace_back(1); stageEnemyNum[0]++;
		}
		else if (pos.z >= 40 && pos.z < 120)
		{
			stageNember_.emplace_back(2); stageEnemyNum[1]++;
		}
		else if (pos.z >= 120 && pos.z < 200)
		{
			stageNember_.emplace_back(3); stageEnemyNum[2]++;
		}
	}
	else if (pos.x >= -120 && pos.x < -40)
	{
		if (pos.z >= -40 && pos.z < 40)
		{
			stageNember_.emplace_back(4); stageEnemyNum[3]++;
		}
		else if (pos.z >= 40 && pos.z < 120)
		{
			stageNember_.emplace_back(5); stageEnemyNum[4]++;
		}
		else if (pos.z >= 120 && pos.z < 200)
		{
			stageNember_.emplace_back(6); stageEnemyNum[5]++;
		}
	}
	else if (pos.x >= -200 && pos.x < -120)
	{
		if (pos.z >= -40 && pos.z < 40)
		{
			stageNember_.emplace_back(7); stageEnemyNum[6]++;
		}
		else if (pos.z >= 40 && pos.z < 120)
		{
			stageNember_.emplace_back(8); stageEnemyNum[7]++;
		}
		else if (pos.z >= 120 && pos.z < 200)
		{
			stageNember_.emplace_back(9); stageEnemyNum[8]++;
		}
	}
	else { stageNember_.emplace_back(9); }
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

		//移動系
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

		//射撃系
	case NormalShotXEnemy:

		break;

	case NormalShotZEnemy:

		break;

	case NormalShotXZEnemy:

		break;

	case HomingShotEnemy:

		break;

		//複合系,特殊系
	case HomingMoveShotEnemy:
		RefVec(i, enemyPos);
		break;
	}

}

void Enemy::OnCollisionToPlayer(int i, XMFLOAT3 playerPos)
{
	//敵同士と同じ挙動
	OnCollisionToEnemy(i, playerPos);
}

void Enemy::OnCollisionToBullet(int i)
{
	//hp減
	hp_[i]--;

	//hp��0�ȉ��Ȃ玀�S
	if (hp_[i] <= 0 && isDead_[i] == false) {
		stageEnemyNum[stageNember_[i]] -= 1;
	//hpが0以下なら死亡
	if (hp_[i] <= 0) {
		isDead_[i] = true;
	}
}

void Enemy::CheckIsDead()
{
	for (int i = 0; i < object.size(); i++) {

		//死亡していたら要素を配列から削除
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
	//敵同士のベクトルを用意
	Vector3 enemy1 = { enemyPos.x,enemyPos.y,enemyPos.z };
	Vector3 enemy2 = { position[i].x,position[i].y,position[i].z };

	//敵同士のベクトルを取る
	Vector3 velocity = enemy1 - enemy2;

	//正規化をして速度をかける
	velocity.normalize();
	velocity *= enemySpeed;

	//移動ベクトルを加算
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

void Enemy::CheckDeadEnemy()
{
	for (int i = 0; i < 9; i++)
	{
		if (stageEnemyNum[i] <= 1)
		{
			stageClearFlag[i] = true;
		}
	}
}

