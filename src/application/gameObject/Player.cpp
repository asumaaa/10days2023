#include "Player.h"
#include "mathOriginal.h"
#include "ImGui.h"

#define G 6.674	//万有引力定数
#define GAcceleration 9.80665 * 1/10	//重力加速度

Camera* Player::camera = nullptr;
Input* Player::input = nullptr;
DXInput* Player::dxInput = nullptr;

void Player::Initialize()
{
	//射撃音
	shotSE = new SoundManager();
	shotSE->SoundLoadWave("Resources/Audio/shotSE.wav");
	//被弾
	damageSE = new SoundManager();
	damageSE->SoundLoadWave("Resources/Audio/damageSE.wav");
}

void Player::Update()
{
	if (!isDead) {

		//無敵時間更新
		if (invTimer) {
			invTimer--;
		}

		//弾更新
		UpdateBullet();

		//動く
		Move();

		//オブジェクト更新
		UpdateObject();
	}

	//ImGui::Begin("PlayerHp");
	//ImGui::Text("%d", HP);
	//ImGui::End();

}

void Player::UpdateObject()
{
	object->SetPosition(position);
	object->SetRotation(rotation0 + rotation1);
	object->SetScale(scale);
}

void Player::UpdateBullet()
{
	if (input->TriggerKey(DIK_RETURN))
	{
		shotSE->StopWave();
		shotSE->SoundPlayWave(false, shotSEVolume);

		//ショットフラグを立てる
		bullet->SetShotFlag(true);

		//弾ベクトル
		XMFLOAT3 bulletVelocity = rollRotation(XMFLOAT3(0.0f, 0.0f, 1.0f), rotation1);

		//弾生成場所とvelocityをセット
		bullet->SetBullet(position, bulletVelocity);
	}
	bullet->Update();
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	bullet->Draw(cmdList);
}

void Player::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	bullet->DrawLightView(cmdList);
}

void Player::Move()
{
	//ジャンプ更新
	//UpdateJump();

	//重力更新
	/*UpdateGravity();*/

	//キー操作
	KeyControl();
}

void Player::KeyControl()
{
	//AROWキーで角度変更
	rotVelocity.y = (input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT)) * rotSpeed;
	//角度ベクトルを加算
	rotation1 = rotation1 + rotVelocity;

	//ASDWで移動
	posVelocity.x = (input->PushKey(DIK_D) - input->PushKey(DIK_A)) * posSpeed;
	posVelocity.z = (input->PushKey(DIK_W) - input->PushKey(DIK_S)) * posSpeed;
	//進行ベクトルを回転
	posVelocity = rollRotation(posVelocity, rotation1);

	position.x += posVelocity.x;
	position.z += posVelocity.z;

}

void Player::UpdateGravity()
{
	//接地していたらタイマーとベクトルリセット
	if (groundFlag == true)
	{
		fallTimer = 0.0f;
		fallVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	//接地していなければ
	if (groundFlag == false)
	{
		//落下タイマーが最大値より小さければ
		if (fallTimer < fallTime)
		{
			fallTimer += fallFrame;
		}
	}

	//落下ベクトル計算
	fallVelocity.y = -(GAcceleration * fallTimer);

	//座標に落下ベクトルを加算
	position = position + fallVelocity;
}

void Player::UpdateJump()
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

void Player::UpdateAttack()
{
}

void Player::SetObject(FbxObject3D* object)
{
	//引数のオブジェクトをセット
	Player::object.reset(object);

	position = object->GetPosition();
	rotation0 = object->GetRotation();
	scale = object->GetScale();
}

void Player::SetSRV(ID3D12DescriptorHeap* SRV)
{
	bullet->SetSRV(SRV);
}

void Player::HitPlane()
{
	//接地フラグを立てる
	groundFlag = true;

	//めり込まなくなるまで加算
	position.y += 0.1f;

	//オブジェクト更新
	UpdateObject();
}

void Player::HitEnemy()
{
	//無敵時間ではなかったらhpを減らし無敵時間追加
	if (!invTimer) {
		damageSE->StopWave();
		damageSE->SoundPlayWave(false, damageSEVolume);
		HP--;
		invTimer = InvTime;
	}
	//HPが0以下なら死亡
	if (!HP) {
		isDead = true;
	}
}

void Player::Reset()
{
	HP = 10;
	isDead = false;
}
