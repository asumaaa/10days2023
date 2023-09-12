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
}

void Enemy::Update(XMFLOAT3 playerPos)
{
	//プレイヤー座標取得
	this->playerPosition = playerPos;

	//動き
	TypeUpdate();

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

		switch (type[i]) {

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

			//複合系,特殊系
		case HomingMoveShotEnemy:
			MoveHoming(i);
			ShotHoming(i);
			break;
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

void Enemy::UpdateJump()
{
	//接地していたら
	if (groundFlag == true)
	{
		//スペースキーでジャンプ
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
	//引数のオブジェクトをセット
	Enemy::object.emplace_back(object);

	position.emplace_back(object->GetPosition());
	rotation.emplace_back(object->GetRotation());
	scale.emplace_back(object->GetScale());

	type.push_back(NormalShotXZEnemy);
	moveX.push_back(true);
	moveZ.push_back(true);
}

void Enemy::HitPlane()
{
	////接地フラグを立てる
	//groundFlag = true;

	////めり込まなくなるまで加算
	//position.y += 0.1f;
}