#pragma once
#include "memory"
#include "list"

#include "Input.h"
#include "DXInput.h"
#include "DirectXCommon.h"
#include "Camera.h"
#include "FbxObject3d.h"
#include "FbxModel.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "FireParticle.h"
#include "Light.h"
#include "LightGroup.h"
#include "CSVLoader.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "ColliderCubeObject.h"
#include "ColliderSphereObject.h"
#include "ColliderPlaneObject.h"
#include "ColliderManager.h"
#include "SparkParticle.h"
#include "SparkParticle2.h"
#include "ExplosionParticle1.h"
#include "ExplosionParticle2.h"
#include "BillboardSprite.h"
#include "BillboardSpriteModel.h"

enum Scene {
	TITLE,
	SERECT,
	PLAY,
	MENU,
	GAMEOVER,
	CLEAR
};

class GameScene
{
	//メンバ関数
public:
	GameScene();
	~GameScene();
	//初期化
	void Initialize(DirectXCommon* dxCommon, Input* input);
	//終了時
	void Finalize();
	//更新
	void Update();
	void UpdateCollider();
	void UpdateSprite();
	//シーン切り替え
	void SceneChange();
	//描画
	void Draw();
	void DrawFBXLightView();
	void DrawFBX();
	void DrawCollider();
	void DrawSprite();
	void DrawParticle();


	//敵のオブジェクト削除
	void DeleteEnemy1();
	void DeleteEnemy2();
	void DeleteEnemy3();
	void DeleteEnemy4();
	void DeleteEnemy5();
	void DeleteEnemy6();
	void DeleteEnemy7();
	void DeleteEnemy8();
	void DeleteEnemy9();
  
	//シーン変更でのリセット
	void ResetSceneData();

	//セッター
	void SetSRV(ID3D12DescriptorHeap* SRV);
	//ゲッター
	DirectX::XMMATRIX GetLightViewProjection();

	//メンバ変数
private:
	//デバイスとinput
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DXInput* dxInput = new DXInput();
	//カメラ
	std::unique_ptr<Camera> camera_;

	//シーン
	int scene = TITLE;
	//選択中のボタン
	//0:タイトル	1:ステージセレクト
	int serectScene = 0;
	//0:ステージ1	1:ステージ2		2:ステージ3
	int serectStage = 0;
	//クリアフラグ
	bool isClear = false;

	//fbx
	std::list<std::unique_ptr<FbxModel>> models;

	//レベルエディタ
	std::unique_ptr<JSONLoader> jsonLoader;

	//オブジェクト
	std::list<std::unique_ptr<FbxObject3D>> object;

	//ライト 影用
	std::unique_ptr<Light> light;
	float lightDir[3] = { 0.0f,-1.0f , -1.0f };
	float lightPos[3] = { 0.0f,25.0f,25.0f };
	float lightTarget[3] = { 0.0f,0.0f,0.0f };
	float lightFactorAngle[2] = { 20.0f,30.0f, };
	float lightAtten[3] = { 0.0f,0.0f,0.0f };

	//ライト
	std::unique_ptr<LightGroup> lightGroup;
	float lightManagerDir[3] = { 0.0f,-1.0f , 1.0f };

	//テクスチャマネージャー
	std::unique_ptr <TextureManager> textureManager;

	//変形行列
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rotation0 = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 0.010f,0.010f,0.010f };
	DirectX::XMFLOAT3 rotation1 = { 0.0f,0.0f,0.0f };

	//プレイヤー
	std::unique_ptr<Player> player;
	//プレイヤーの弾
	std::unique_ptr<PlayerBullet>playerBullet;

	//敵
	std::unique_ptr<Enemy>enemy;
	//敵の弾
	std::unique_ptr<EnemyBullet>enemyBullet;

	//平面
	/*std::unique_ptr<Plane> plane;*/

	//スカイドーム
	std::unique_ptr<FbxObject3D> skydome;

	//コライダーのモデル
	std::unique_ptr<ColliderCubeModel>colliderCubeModel;
	std::unique_ptr<ColliderSphereModel>colliderSphereModel;
	std::unique_ptr<ColliderPlaneModel>colliderPlaneModel;

	//コライダー
	std::unique_ptr<ColliderManager> colliderManager;

	//弾けるパーティクル
	std::unique_ptr<SparkParticle>sparkParticle;

	//弾けるパーティクル2
	std::unique_ptr<SparkParticle2>sparkParticle2;

	//爆発パーテイクル1
	std::unique_ptr<ExplosionParticle1>explosionParticle1;

	//爆発パーテイクル2
	std::unique_ptr<ExplosionParticle2>explosionParticle2;

	//描画フラグ
	int drawParticle[1] = { 1 };
	int drawFbx[1] = { 1 };
	int drawSprite[1] = { 0 };
	int drawCollider[1] = { 1 };

	int stageNum = 1;
	//ビルボード　
	std::unique_ptr<BillboardSprite>billboardSprite;
	std::unique_ptr<BillboardSpriteModel>billboardSpriteModel;

	//画像
	std::unique_ptr<Sprite> titleSprite;
	std::unique_ptr<Sprite> gameoverSprite;
	std::unique_ptr<Sprite> clearSprite;
	std::unique_ptr<Sprite> menuSprite;
	std::unique_ptr<Sprite> titleSerectButtonSprite;
	std::unique_ptr<Sprite> stageSerectButtonSprite;

	std::unique_ptr<Sprite> stageSelectSprite;
	std::unique_ptr<Sprite> menuButtonSprite;
	std::unique_ptr<Sprite> menuCloseSprite;
	std::unique_ptr<Sprite> stage1Sprite;
	std::unique_ptr<Sprite> stage2Sprite;
	std::unique_ptr<Sprite> stage3Sprite;
};
