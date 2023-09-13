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
	//繝｡繝ｳ繝宣未謨ｰ
public:
	GameScene();
	~GameScene();
	//蛻晄悄蛹・
	void Initialize(DirectXCommon* dxCommon, Input* input);
	//邨ゆｺ・凾
	void Finalize();
	//譖ｴ譁ｰ
	void Update();
	void UpdateCollider();
	void UpdateSprite();
	//繧ｷ繝ｼ繝ｳ蛻・ｊ譖ｿ縺・
	void SceneChange();
	//謠冗判
	void Draw();
	void DrawFBXLightView();
	void DrawFBX();
	void DrawCollider();
	void DrawSprite();
	void DrawParticle();


	//謨ｵ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝亥炎髯､
	void DeleteEnemy1();
	void DeleteEnemy2();
	void DeleteEnemy3();
	void DeleteEnemy4();
	void DeleteEnemy5();
	void DeleteEnemy6();
	void DeleteEnemy7();
	void DeleteEnemy8();
	void DeleteEnemy9();
  
	//繧ｷ繝ｼ繝ｳ螟画峩縺ｧ縺ｮ繝ｪ繧ｻ繝・ヨ
	void ResetSceneData();

	//繧ｻ繝・ち繝ｼ
	void SetSRV(ID3D12DescriptorHeap* SRV);
	//繧ｲ繝・ち繝ｼ
	DirectX::XMMATRIX GetLightViewProjection();

	//繝｡繝ｳ繝仙､画焚
private:
	//繝・ヰ繧､繧ｹ縺ｨinput
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DXInput* dxInput = new DXInput();
	//繧ｫ繝｡繝ｩ
	std::unique_ptr<Camera> camera_;

	//繧ｷ繝ｼ繝ｳ
	int scene = TITLE;
	//驕ｸ謚樔ｸｭ縺ｮ繝懊ち繝ｳ
	//0:繧ｿ繧､繝医Ν	1:繧ｹ繝・・繧ｸ繧ｻ繝ｬ繧ｯ繝・
	int serectScene = 0;
	//0:繧ｹ繝・・繧ｸ1	1:繧ｹ繝・・繧ｸ2		2:繧ｹ繝・・繧ｸ3
	int serectStage = 0;
	//繧ｯ繝ｪ繧｢繝輔Λ繧ｰ
	bool isClear = false;

	//fbx
	std::list<std::unique_ptr<FbxModel>> models;

	//繝ｬ繝吶Ν繧ｨ繝・ぅ繧ｿ
	std::unique_ptr<JSONLoader> jsonLoader;

	//繧ｪ繝悶ず繧ｧ繧ｯ繝・
	std::list<std::unique_ptr<FbxObject3D>> object;

	//繝ｩ繧､繝・蠖ｱ逕ｨ
	std::unique_ptr<Light> light;
	float lightDir[3] = { 0.0f,-1.0f , -1.0f };
	float lightPos[3] = { 0.0f,25.0f,25.0f };
	float lightTarget[3] = { 0.0f,0.0f,0.0f };
	float lightFactorAngle[2] = { 20.0f,30.0f, };
	float lightAtten[3] = { 0.0f,0.0f,0.0f };

	//繝ｩ繧､繝・
	std::unique_ptr<LightGroup> lightGroup;
	float lightManagerDir[3] = { 0.0f,-1.0f , 1.0f };

	//繝・け繧ｹ繝√Ε繝槭ロ繝ｼ繧ｸ繝｣繝ｼ
	std::unique_ptr <TextureManager> textureManager;

	//螟牙ｽ｢陦悟・
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 rotation0 = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 0.010f,0.010f,0.010f };
	DirectX::XMFLOAT3 rotation1 = { 0.0f,0.0f,0.0f };

	//繝励Ξ繧､繝､繝ｼ
	std::unique_ptr<Player> player;
	//繝励Ξ繧､繝､繝ｼ縺ｮ蠑ｾ
	std::unique_ptr<PlayerBullet>playerBullet;

	//謨ｵ
	std::unique_ptr<Enemy>enemy;
	//謨ｵ縺ｮ蠑ｾ
	std::unique_ptr<EnemyBullet>enemyBullet;

	//蟷ｳ髱｢
	/*std::unique_ptr<Plane> plane;*/

	//繧ｳ繝ｩ繧､繝繝ｼ縺ｮ繝｢繝・Ν
	std::unique_ptr<ColliderCubeModel>colliderCubeModel;
	std::unique_ptr<ColliderSphereModel>colliderSphereModel;
	std::unique_ptr<ColliderPlaneModel>colliderPlaneModel;

	//繧ｳ繝ｩ繧､繝繝ｼ
	std::unique_ptr<ColliderManager> colliderManager;

	//蠑ｾ縺代ｋ繝代・繝・ぅ繧ｯ繝ｫ
	std::unique_ptr<SparkParticle>sparkParticle;

	//蠑ｾ縺代ｋ繝代・繝・ぅ繧ｯ繝ｫ2
	std::unique_ptr<SparkParticle2>sparkParticle2;

	//辷・匱繝代・繝・う繧ｯ繝ｫ1
	std::unique_ptr<ExplosionParticle1>explosionParticle1;

	//辷・匱繝代・繝・う繧ｯ繝ｫ2
	std::unique_ptr<ExplosionParticle2>explosionParticle2;

	//謠冗判繝輔Λ繧ｰ
	int drawParticle[1] = { 1 };
	int drawFbx[1] = { 1 };
	int drawSprite[1] = { 0 };
	int drawCollider[1] = { 1 };

	//繝薙Ν繝懊・繝峨
	std::unique_ptr<BillboardSprite>billboardSprite;
	std::unique_ptr<BillboardSpriteModel>billboardSpriteModel;

	//現在のステージ
	int stageNum = 1;
	//逕ｻ蜒・
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
