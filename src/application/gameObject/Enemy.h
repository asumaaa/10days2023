#pragma once
#include "DirectXMath.h"
#include "FbxObject3D.h"
#include "Sprite.h"
#include "EnemyBullet.h"

enum EnemyType {
	MoveXEnemy,
	MoveZEnemy,
	MoveXZEnemy,
	HomingMoveEnemy,
	NormalShotXEnemy,
	NormalShotZEnemy,
	NormalShotXZEnemy,
	HomingShotEnemy,
	HomingMoveShotEnemy,
};

class Enemy
{
private:	//エイリアス
	//Microsoft::WRL::を省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	//メンバ関数
public:
	//静的メンバ関数
	static void SetCamera(Camera* camera) { Enemy::camera = camera; }
	static void SetInput(Input* input) { Enemy::input = input; }
	static void SetDXInput(DXInput* dxInput) { Enemy::dxInput = dxInput; }

	//初期化
	void Initialize();
	//更新
	void Update(XMFLOAT3 playerPos);
	void UpdateObject();
	void UpdateSprite();
	//描画
	void DrawSprite(ID3D12GraphicsCommandList* cmdList);
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);
	void SetSRV(ID3D12DescriptorHeap* SRV);

	//挙動関連
	//挙動全般
	void TypeUpdate();

	//移動種類
	void MoveHoming(int i);
	void MoveX(int i);
	void MoveZ(int i);

	//射撃
	void Shot(int i,XMFLOAT3 velosity);
	void ShotX(int i);
	void ShotZ(int i);
	void ShotHoming(int i);

	//落下
	void UpdateGravity();
	//ジャンプ
	void UpdateJump();

	//攻撃系全般
	void UpdateAttack();

	//セッター
	void SetObject(FbxObject3D* object);
	void HitPlane();

	//void SetType(int num,int type) { this->type[num] = type; }
	void SetStageMid(XMFLOAT3 stageMid) { this->stageMid = stageMid; }
	void SetBullet(EnemyBullet* enemyBullet) { Enemy::bullet = enemyBullet; }

	//ゲッター
	XMFLOAT3 GetPosition(int num) { return position[num]; }
	XMFLOAT3 GetRotation(int num) { return rotation[num]; }
	XMFLOAT3 GetScale(int num) { return scale[num]; }

	//静的メンバ変数
private:
	//カメラ
	static Camera* camera;
	//キーボード
	static Input* input;
	//コントローラー
	static DXInput* dxInput;

	//メンバ変数
public:

	//オブジェクト
	std::list<std::unique_ptr<FbxObject3D>>object;
	//スプライト
	std::unique_ptr<Sprite>spriteHpBar;

	//変形行列
	//座標
	std::vector<XMFLOAT3> position;
	//回転
	std::vector<XMFLOAT3> rotation;
	//サイズ
	std::vector<XMFLOAT3> scale;
	//種類
	std::vector<int> type;

	//プレイヤーの位置
	XMFLOAT3 playerPosition = {};
	//ステージの中心座標
	XMFLOAT3 stageMid = {};
	//ステージのサイズ
	XMFLOAT3 stageSize = {40,1,40};

	//動き 0 = 左,下 1 = 右,上 
	std::vector<bool> moveX;
	std::vector<bool> moveZ;
	//移動スピード
	float enemySpeed = 0.1;

	//弾
	EnemyBullet* bullet;
	float bulletSpeed = 0.2;
	//射撃
	const int ShotCoolTime = 100;
	int shotCoolTimer = 0;
	//射撃フラグ
	bool isShot = false;

	//当たり判定関連
	//接地フラグ
	bool groundFlag = false;


	//挙動関連
	
	//落下
	//落下ベクトル
	XMFLOAT3 fallVelocity = { 0.0f,0.0f,0.0f };
	//落下タイマー
	float fallTimer = 0.0f;
	//落下最大値までにかかる時間
	float fallTime = 1.0f;
	//1フレームあたりの落下量
	float fallFrame = 1.0f / 60.0f;

	//ジャンプ
	float jumpHeight = 0.4;

	//スピード
	float speed = 0.15f;

	//HP
	float HP = 100;
};

