#include "GameScene.h"
#include "FbxLoader.h"
#include<fstream>
#include "sstream"
#include "stdio.h"
#include "string.h"
#include "vector"
#include "imgui.h"
#include <iostream>

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	this->dxCommon_ = dxCommon;
	this->input_ = input;

	//�e�N�X�`���}�l�[�W���[
	TextureManager::SetDevice(dxCommon->GetDevice());
	TextureManager* newTextureManager = new TextureManager();
	newTextureManager->Initialize();
	newTextureManager->LoadFile(0, L"Resources/pictures/white1x1.png");
	newTextureManager->LoadFile(1, L"Resources/pictures/toriko2.png");
	newTextureManager->LoadFile(2, L"Resources/pictures/GourmetSpyzer.png");
	newTextureManager->LoadFile(3, L"Resources/pictures/orange.png");
	newTextureManager->LoadFile(4, L"Resources/pictures/red.png");
	newTextureManager->LoadFile(5, L"Resources/pictures/effect1.png");
	newTextureManager->LoadFile(6, L"Resources/pictures/effect2.png");
	newTextureManager->LoadFile(7, L"Resources/pictures/effect3.png");
	newTextureManager->LoadFile(8, L"Resources/pictures/enemyHP.png");
	newTextureManager->LoadFile(9, L"Resources/pictures/toriko.png");
	newTextureManager->LoadFile(10, L"Resources/pictures/grassFiled.png");
	newTextureManager->LoadFile(11, L"Resources/pictures/gravel.png");
	newTextureManager->LoadFile(12, L"Resources/pictures/DissolveMap.png");
	newTextureManager->LoadFile(13, L"Resources/pictures/mapping.png");
	newTextureManager->LoadFile(14, L"Resources/pictures/black.png");
	newTextureManager->LoadFile(15, L"Resources/pictures/blackParticle.png");
	newTextureManager->LoadFile(16, L"Resources/pictures/effect4.png");

	newTextureManager->LoadFile(17, L"Resources/pictures/title.png");
	newTextureManager->LoadFile(18, L"Resources/pictures/gameover.png");
	newTextureManager->LoadFile(19, L"Resources/pictures/clear.png");
	newTextureManager->LoadFile(20, L"Resources/pictures/titleSerectButton.png");
	newTextureManager->LoadFile(21, L"Resources/pictures/stageSelectButton.png");
	newTextureManager->LoadFile(22, L"Resources/pictures/menu.png");

	newTextureManager->LoadFile(23, L"Resources/pictures/stageSelect.png");
	newTextureManager->LoadFile(24, L"Resources/pictures/menuButton.png");
	newTextureManager->LoadFile(25, L"Resources/pictures/menuClose.png");
	newTextureManager->LoadFile(26, L"Resources/pictures/stage1.png");
	newTextureManager->LoadFile(27, L"Resources/pictures/stage2.png");
	newTextureManager->LoadFile(28, L"Resources/pictures/stage3.png");

	textureManager.reset(newTextureManager);

	//FBX���[�_�[������
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//�e�N�X�`���}�l�[�W���[�Z�b�g
	FbxModel::SetTextureManager(textureManager.get());
	FbxModel::SetDevice(dxCommon_->GetDevice());
	//���f�������w�肵�ăt�@�C���ǂݍ���
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("plane"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("player"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("enemy"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("playerBullet"));
	models.emplace_back(FbxLoader::GetInstance()->LoadModelFromFile("sphere"));

	//�X�v���C�g
	Sprite::SetDevice(dxCommon->GetDevice());
	Sprite::SetSpriteManager(textureManager.get());
	Sprite::CreateGraphicsPipeLine();

	//----------�摜----------
	//�^�C�g��
	Sprite* newTitleSprite = new Sprite();
	newTitleSprite->SetTextureNum(17);
	newTitleSprite->Initialize();
	titleSprite.reset(newTitleSprite);
	titleSprite->SetAlpha(1.0f);
	titleSprite->SetScale({ 1280.0f, 720.0f });
	titleSprite->SetPosition({ 0.0f, 0.0 });

	//�Q�[���I�[�o�[
	Sprite* newGameoverSprite = new Sprite();
	newGameoverSprite->SetTextureNum(18);
	newGameoverSprite->Initialize();
	gameoverSprite.reset(newGameoverSprite);
	gameoverSprite->SetAlpha(1.0f);
	gameoverSprite->SetScale({ 1280.0f, 720.0f });
	gameoverSprite->SetPosition({ 0.0f, 0.0 });

	//�N���A
	Sprite* newClearSprite = new Sprite();
	newClearSprite->SetTextureNum(19);
	newClearSprite->Initialize();
	clearSprite.reset(newClearSprite);
	clearSprite->SetAlpha(1.0f);
	clearSprite->SetScale({ 1280.0f, 720.0f });
	clearSprite->SetPosition({ 0.0f, 0.0 });

	//�^�C�g���I��
	Sprite* newTitleSerectButtonSprite = new Sprite();
	newTitleSerectButtonSprite->SetTextureNum(20);
	newTitleSerectButtonSprite->Initialize();
	titleSerectButtonSprite.reset(newTitleSerectButtonSprite);
	titleSerectButtonSprite->SetAlpha(1.0f);
	titleSerectButtonSprite->SetScale({ 1280.0f, 720.0f });
	titleSerectButtonSprite->SetPosition({ 0.0f, 0.0 });

	//�X�e�[�W�I��
	Sprite* newStageSerectButtonSprite = new Sprite();
	newStageSerectButtonSprite->SetTextureNum(21);
	newStageSerectButtonSprite->Initialize();
	stageSerectButtonSprite.reset(newStageSerectButtonSprite);
	stageSerectButtonSprite->SetAlpha(1.0f);
	stageSerectButtonSprite->SetScale({ 1280.0f, 720.0f });
	stageSerectButtonSprite->SetPosition({ 0.0f, 0.0 });

	//���j���[
	Sprite* newMenuSprite = new Sprite();
	newMenuSprite->SetTextureNum(22);
	newMenuSprite->Initialize();
	menuSprite.reset(newMenuSprite);
	menuSprite->SetAlpha(1.0f);
	menuSprite->SetScale({ 1280.0f, 720.0f });
	menuSprite->SetPosition({ 0.0f, 0.0 });

	//�X�e�[�W�Z���N�g
	Sprite* newStageSelectSprite = new Sprite();
	newStageSelectSprite->SetTextureNum(23);
	newStageSelectSprite->Initialize();
	stageSelectSprite.reset(newStageSelectSprite);
	stageSelectSprite->SetAlpha(1.0f);
	stageSelectSprite->SetScale({ 1280.0f, 720.0f });
	stageSelectSprite->SetPosition({ 0.0f, 0.0 });

	//���j���[�J��
	Sprite* newMenuButtonSprite = new Sprite();
	newMenuButtonSprite->SetTextureNum(24);
	newMenuButtonSprite->Initialize();
	menuButtonSprite.reset(newMenuButtonSprite);
	menuButtonSprite->SetAlpha(1.0f);
	menuButtonSprite->SetScale({ 1280.0f, 720.0f });
	menuButtonSprite->SetPosition({ 0.0f, 0.0 });

	//���j���[����
	Sprite* newMenuCloseSprite = new Sprite();
	newMenuCloseSprite->SetTextureNum(25);
	newMenuCloseSprite->Initialize();
	menuCloseSprite.reset(newMenuCloseSprite);
	menuCloseSprite->SetAlpha(1.0f);
	menuCloseSprite->SetScale({ 1280.0f, 720.0f });
	menuCloseSprite->SetPosition({ 0.0f, 0.0 });

	//�X�e�[�W1
	Sprite* newStage1Sprite = new Sprite();
	newStage1Sprite->SetTextureNum(26);
	newStage1Sprite->Initialize();
	stage1Sprite.reset(newStage1Sprite);
	stage1Sprite->SetAlpha(1.0f);
	stage1Sprite->SetScale({ 1280.0f, 720.0f });
	stage1Sprite->SetPosition({ 0.0f, 0.0 });

	//�X�e�[�W2
	Sprite* newStage2Sprite = new Sprite();
	newStage2Sprite->SetTextureNum(27);
	newStage2Sprite->Initialize();
	stage2Sprite.reset(newStage2Sprite);
	stage2Sprite->SetAlpha(1.0f);
	stage2Sprite->SetScale({ 1280.0f, 720.0f });
	stage2Sprite->SetPosition({ 0.0f, 0.0 });

	//�X�e�[�W3
	Sprite* newStage3Sprite = new Sprite();
	newStage3Sprite->SetTextureNum(28);
	newStage3Sprite->Initialize();
	stage3Sprite.reset(newStage3Sprite);
	stage3Sprite->SetAlpha(1.0f);
	stage3Sprite->SetScale({ 1280.0f, 720.0f });
	stage3Sprite->SetPosition({ 0.0f, 0.0 });

	//�J����������
	Camera::SetInput(input_);
	Camera::SetDXInput(dxInput);
	Camera* newCamera = new Camera();
	newCamera->Initialize();
	camera_.reset(newCamera);
	camera_->SetEye({ 0.0f,10.0f,5.0f });
	camera_->SetTarget({ 0.0f,0.0f,0.0f });

	//���C�g(�e)
	Light* newLight = new Light();
	newLight->Initialize();
	light.reset(newLight);

	//���C�g
	LightGroup::StaticInitialize(dxCommon_->GetDevice());
	LightGroup* newLightGroup = new LightGroup();
	newLightGroup = LightGroup::Create();
	lightGroup.reset(newLightGroup);

	//�R���C�_�[�̃L���[�u
	ColliderCubeModel* newCubeModel = new ColliderCubeModel();
	newCubeModel->CreateBuffers(dxCommon_->GetDevice());
	colliderCubeModel.reset(newCubeModel);
	ColliderCubeObject::SetDevice(dxCommon_->GetDevice());
	ColliderCubeObject::SetCamera(camera_.get());
	ColliderCubeObject::SetInput(input_);
	ColliderCubeObject::SetModel(colliderCubeModel.get());
	ColliderCubeObject::CreateGraphicsPipeline();

	//�R���C�_�[�̋�
	ColliderSphereModel* newSphereModel = new ColliderSphereModel();
	newSphereModel->CreateBuffers(dxCommon_->GetDevice());
	colliderSphereModel.reset(newSphereModel);
	ColliderSphereObject::SetDevice(dxCommon_->GetDevice());
	ColliderSphereObject::SetCamera(camera_.get());
	ColliderSphereObject::SetInput(input_);
	ColliderSphereObject::SetModel(colliderSphereModel.get());
	ColliderSphereObject::CreateGraphicsPipeline();

	//�R���C�_�[�̕���
	ColliderPlaneModel* newPlaneModel = new ColliderPlaneModel();
	newPlaneModel->CreateBuffers(dxCommon_->GetDevice());
	colliderPlaneModel.reset(newPlaneModel);
	ColliderPlaneObject::SetDevice(dxCommon_->GetDevice());
	ColliderPlaneObject::SetCamera(camera_.get());
	ColliderPlaneObject::SetInput(input_);
	ColliderPlaneObject::SetModel(colliderPlaneModel.get());
	ColliderPlaneObject::CreateGraphicsPipeline();

	//�R���C�_�[�}�l�[�W���[
	ColliderManager::SetColliderCubeModel(colliderCubeModel.get());
	ColliderManager::SetColliderSphereModel(colliderSphereModel.get());
	ColliderManager* newColliderManager = new ColliderManager();
	colliderManager.reset(newColliderManager);

	//�e����p�[�e�B�N��
	SparkParticle::SetSpriteManager(textureManager.get());
	SparkParticle::SetDevice(dxCommon_->GetDevice());
	SparkParticle::SetCamera(camera_.get());
	SparkParticle::SetInput(input_);
	SparkParticle::CreateGraphicsPipeline();
	SparkParticle* newSparkParticle = new SparkParticle();
	newSparkParticle->CreateBuffers();
	newSparkParticle->SetTextureNum(5);
	sparkParticle.reset(newSparkParticle);

	//�e����p�[�e�B�N��
	SparkParticle2::SetSpriteManager(textureManager.get());
	SparkParticle2::SetDevice(dxCommon_->GetDevice());
	SparkParticle2::SetCamera(camera_.get());
	SparkParticle2::SetInput(input_);
	SparkParticle2::CreateGraphicsPipeline();
	SparkParticle2* newSparkParticle2 = new SparkParticle2();
	newSparkParticle2->CreateBuffers();
	newSparkParticle2->SetTextureNum(3);
	sparkParticle2.reset(newSparkParticle2);

	//�����p�[�e�B�N��
	ExplosionParticle1::SetSpriteManager(textureManager.get());
	ExplosionParticle1::SetDevice(dxCommon_->GetDevice());
	ExplosionParticle1::SetCamera(camera_.get());
	ExplosionParticle1::SetInput(input_);
	ExplosionParticle1::CreateGraphicsPipeline();
	ExplosionParticle1* newExplosionParticle1 = new ExplosionParticle1();
	newExplosionParticle1->CreateBuffers();
	newExplosionParticle1->SetTextureNum(16);
	explosionParticle1.reset(newExplosionParticle1);

	//�����p�[�e�B�N��
	ExplosionParticle2::SetSpriteManager(textureManager.get());
	ExplosionParticle2::SetDevice(dxCommon_->GetDevice());
	ExplosionParticle2::SetCamera(camera_.get());
	ExplosionParticle2::SetInput(input_);
	ExplosionParticle2::CreateGraphicsPipeline();
	ExplosionParticle2* newExplosionParticle2 = new ExplosionParticle2();
	newExplosionParticle2->CreateBuffers();
	newExplosionParticle2->SetTextureNum(16);
	explosionParticle2.reset(newExplosionParticle2);

	//�r���{�[�h�̃X�v���C�g
	BillboardSpriteModel::SetDevice(dxCommon_->GetDevice());
	BillboardSpriteModel::SetSpriteManager(textureManager.get());
	BillboardSpriteModel* newBillboardSpriteModel = new BillboardSpriteModel();
	newBillboardSpriteModel->CreateBuffers(dxCommon_->GetDevice());
	newBillboardSpriteModel->SetTextureNum(8);
	billboardSpriteModel.reset(newBillboardSpriteModel);
	BillboardSprite::SetDevice(dxCommon_->GetDevice());
	BillboardSprite::SetCamera(camera_.get());
	BillboardSprite::SetInput(input_);
	BillboardSprite::SetModel(billboardSpriteModel.get());
	BillboardSprite::CreateGraphicsPipeline();
	BillboardSprite* newBillboardSprite = new BillboardSprite();
	newBillboardSprite->Initialize();
	billboardSprite.reset(newBillboardSprite);

	//�v���C���[�̒e
	PlayerBullet::SetCamera(camera_.get());
	PlayerBullet::SetInput(input);
	PlayerBullet* newPlayerBullet = new PlayerBullet();
	//���f���Z�b�g
	for (std::unique_ptr<FbxModel>& model : models)
	{
		if (model->GetFileName() == "playerBullet")
		{
			newPlayerBullet->SetModel(model.get());
		}
	}
	newPlayerBullet->Initialize();
	playerBullet.reset(newPlayerBullet);

	//�v���C���[
	Player::SetCamera(camera_.get());
	Player::SetInput(input);
	/*Player::SetDXInput()*/
	Player* newPlayer = new Player();
	newPlayer->Initialize();
	newPlayer->SetBullet(playerBullet.get());
	player.reset(newPlayer);

	//�G�e
	EnemyBullet::SetCamera(camera_.get());
	EnemyBullet::SetInput(input);
	EnemyBullet* newEnemyBullet = new EnemyBullet();
	//���f���Z�b�g
	for (std::unique_ptr<FbxModel>& model : models)
	{
		if (model->GetFileName() == "playerBullet")
		{
			newEnemyBullet->SetModel(model.get());
		}
	}
	newEnemyBullet->Initialize();
	enemyBullet.reset(newEnemyBullet);

	//�G
	Enemy::SetCamera(camera_.get());
	Enemy::SetInput(input);
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize();
	newEnemy->SetBullet(enemyBullet.get());
	enemy.reset(newEnemy);
	int enemyNum = 0;

	//����
	/*Plane::SetCamera(camera_.get());
	Plane::SetInput(input);*/
	/*Plane* newPlane = new Plane();
	newPlane->Initialize();
	plane.reset(newPlane);*/

	//�f�o�C�X���Z�b�g
	FbxObject3D::SetDevice(dxCommon_->GetDevice());
	FbxObject3D::SetCamera(camera_.get());
	FbxObject3D::SetLight(light.get());
	FbxObject3D::SetLightGroup(lightGroup.get());
	FbxObject3D::CreateGraphicsPipelineLightView();
	FbxObject3D::CreateGraphicsPipeline();

	//���x���G�f�B�^
	JSONLoader* newJsonLoader = new JSONLoader();
	newJsonLoader->LoadFile("Resources/json/10days.json");
	jsonLoader.reset(newJsonLoader);

	for (int i = 0; i < jsonLoader->GetObjectNum(); i++)
	{
		std::unique_ptr<FbxObject3D>newObject = std::make_unique<FbxObject3D>();
		//�I�u�W�F�N�g������
		newObject->Initialize();

		//���f���Z�b�g
		for (std::unique_ptr<FbxModel>& model : models)
		{
			if (jsonLoader->GetFileName(i) == model->GetFileName())
			{
				newObject->SetModel(model.get());
			}
			//�X�e�[�W�̏ꍇ
			if (model->GetFileName() == "plane")
			{
				if (jsonLoader->GetFileName(i) == "stage1" || jsonLoader->GetFileName(i) == "stage2" ||
					jsonLoader->GetFileName(i) == "stage3" || jsonLoader->GetFileName(i) == "stage4" ||
					jsonLoader->GetFileName(i) == "stage5" || jsonLoader->GetFileName(i) == "stage6" ||
					jsonLoader->GetFileName(i) == "stage7" || jsonLoader->GetFileName(i) == "stage8" ||
					jsonLoader->GetFileName(i) == "stage9")
				{
					newObject->SetModel(model.get());
				}
			}
		}

		//�I�u�W�F�N�g�̔z�u
		newObject->SetObjectData(jsonLoader->GetObjectData(i));
		//�R���C�_�[�̔z�u
		newObject->SetColliderData(jsonLoader->GetColliderData(i));
		//�e�N�X�`���f�[�^�̃Z�b�g
		newObject->SetTextureData(jsonLoader->GetTextureData(i));

		object.push_back(std::move(newObject));

		//�v���C���[�̃I�u�W�F�N�g����������
		if (jsonLoader->GetFileName(i) == "player")
		{
			player->SetObject(object.back().get());
			/*object.pop_back();*/
		}
		//�G�̃I�u�W�F�N�g����������
		if (jsonLoader->GetFileName(i) == "enemy")
		{
			enemy->SetObject(object.back().get());

			//�G�ԍ��Z�b�g
			object.back().get()->SetEnemyNum(enemyNum);
			enemyNum++;
		}
		//���ʂ̃I�u�W�F�N�g����������
	/*	if (jsonLoader->GetFileName(i) == "plane")
		{
			plane->SetObject(object.back().get());
		}*/

		//�R���C�_�[�̃Z�b�g
		ColliderManager::SetCollider(jsonLoader->GetColliderData(i));
	}
}

void GameScene::Finalize()
{
	models.clear();
	delete textureManager.get();
	delete lightGroup.get();
	object.clear();
	/*delete tree1csv;
	delete spriteManager;
	delete lightGroup;*/
}

void GameScene::Update()
{
	//�V�[���؂�ւ�
	SceneChange();

	ImGui::Begin("scene");
	ImGui::Text("%d", scene);
	ImGui::End();

	//�J�����X�V
	camera_->UpdatePlayer(player->GetPosition(), player->GetRotation1());
	//camera_->DebugUpdate();
	camera_->Update();
	//�R���g���[���[�X�V
	dxInput->InputProcess();

	billboardSprite->SetPosition(XMFLOAT3(0.0f, 10.0f, 0.0f));
	billboardSprite->SetScale(XMFLOAT3(2.5f, 0.3f, 1.0f));
	billboardSprite->Update();

	//�X�v���C�g�X�V
	UpdateSprite();

	switch (scene) {

	case PLAY:

		/*particleObject->SetPosition(XMFLOAT3(10.0f,5.0f,0));*/
		//�p�[�e�B�N��
		/*particleManager->Update();*/
		if (input_->TriggerKey(DIK_N))
		{
			/*sparkParticle->Add(XMFLOAT3(0,0,0));*/
		}
		sparkParticle->Update();

		if (input_->TriggerKey(DIK_N))
		{
			sparkParticle2->Add(XMFLOAT3(0, 3.0f, 0));
			explosionParticle1->Add(XMFLOAT3(0, 3.0f, 0));
			explosionParticle2->Add(XMFLOAT3(0, 3.0f, 0));
		}
		sparkParticle2->Update();
		explosionParticle1->Update();
		explosionParticle2->Update();

		//���C�g
		light->SetEye(XMFLOAT3(lightPos));
		light->SetTarget(XMFLOAT3(lightTarget));
		light->SetDir(XMFLOAT3(lightDir));
		light->Update();

		//���C�g
		lightGroup->SetAmbientColor(XMFLOAT3(1, 1, 1));
		lightGroup->SetDirLightActive(0, false);
		lightGroup->SetDirLightActive(1, false);
		lightGroup->SetDirLightActive(2, false);
		lightGroup->Update();

		//�v���C���[
		player->Update();

		//�G
		enemy->Update(player->GetPosition());

		//����
		/*plane->Update();*/

		//�X�y�[�X�L�[�Ńt�@�C���ǂݍ��ݍX�V
		//if(input_->TriggerKey(DIK_SPACE))
		//{
		//	jsonLoader->LoadFile("Resources/json/demo1.json"); 
		//	int i = 0;
		//	for (std::unique_ptr<FbxObject3D>& object0 : object)
		//	{
		//		//�v���C���[�ȊO�̃I�u�W�F�N�g�X�V
		//		if (object0->GetFileName() != "player")
		//		{
		//			object0->SetPosition(jsonLoader->GetPosition(i));
		//			object0->SetScale(jsonLoader->GetScale(i));
		//			object0->SetRotation(jsonLoader->GetRotation(i));
		//		}
		//		i++;
		//	}
		//}

		for (std::unique_ptr<FbxObject3D>& object0 : object)
		{
			object0->Update();
		}

		//�R���C�_�[�X�V
		UpdateCollider();

		break;
	}

}

void GameScene::UpdateCollider()
{
	//���O����
	ColliderManager::PreUpdate();

	//�v���C���[�ƕ��ʂƂ̔���
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		if (object0->GetFileName() == "player")
		{
			for (std::unique_ptr<FbxObject3D>& object1 : object)
			{
				if (object1->GetColliderType() == "Plane")
				{
					//�������Ă�����
					while (ColliderManager::CheckCollider(object0->GetColliderData(), object1->GetColliderData()))
					{
						player->HitPlane();
					}
				}
			}
		}
	}

	//�v���C���[�ƓG�e�Ƃ̔���
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		if (object0->GetFileName() == "player")
		{
			//�e����ȏ゠���
			if (enemyBullet->GetBulletNum() >= 1)
			{
				for (int i = 0; i < enemyBullet->GetBulletNum(); i++)
				{
					if (ColliderManager::CheckCollider(enemyBullet->GetColliderData(i),
						object0->GetColliderData()))
					{
						//�p�[�e�B�N��
						sparkParticle2->Add(XMFLOAT3(enemyBullet->GetPosition(i)));
						explosionParticle1->Add(XMFLOAT3(enemyBullet->GetPosition(i)));
						explosionParticle2->Add(XMFLOAT3(enemyBullet->GetPosition(i)));
						//�e
						enemyBullet->SetHitFlag(true, i);

						//�v���C���[��e����
						player->HitEnemy();
					}
				}
			}
		}
	}

	int enemyNum = 0;

	//�v���C���[�ƓG�Ƃ̔���
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		if (object0->GetFileName() == "player")
		{
			for (std::unique_ptr<FbxObject3D>& object1 : object)
			{
				if (object1->GetFileName() == "enemy")
				{
					if (!enemy->GetIsDead(enemyNum)) {
						//�������Ă�����
						if (ColliderManager::CheckCollider(object0->GetColliderData(), object1->GetColliderData()))
						{
							enemy->OnCollisionToEnemy(object1->GetEnemyNum(), player->GetPosition());
							//�v���C���[��e����
							player->HitEnemy();
						}
					}
					enemyNum++;
				}
			}
		}
	}

	int objectNum = 0;
	enemyNum = 0;

	//�e�ƓG�Ƃ̔���
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		if (object0->GetFileName() == "enemy")
		{
			//�e����ȏ゠���
			if (playerBullet->GetBulletNum() >= 1)
			{
				if (!enemy->GetIsDead(enemyNum)) {
					for (int i = 0; i < playerBullet->GetBulletNum(); i++)
					{
						if (ColliderManager::CheckCollider(playerBullet->GetColliderData(i),
							object0->GetColliderData()))
						{
							//�p�[�e�B�N��
							sparkParticle2->Add(XMFLOAT3(playerBullet->GetPosition(i)));
							explosionParticle1->Add(XMFLOAT3(playerBullet->GetPosition(i)));
							explosionParticle2->Add(XMFLOAT3(playerBullet->GetPosition(i)));
							//�e
							playerBullet->SetHitFlag(true, i);

							//�G�����蔻�菈��
							enemy->OnCollisionToBullet(object0->GetEnemyNum());
						}
					}
				}
			}
			enemyNum++;
		}

		objectNum++;
	}

	//for (int i = 0; i < enemy->GetSize();i++) {
	//	if (enemy->GetIsDead(i)) {
	//		object.erase(std::next(object.begin(), objectNum));
	//	}
	//}

	//�G���m�̔���
	//�v���C���[�ƕ��ʂƂ̔���
	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		if (object0->GetFileName() == "enemy")
		{

			//����ł����珜�O
			if (enemy->GetIsDead(object0->GetEnemyNum())) {
				break;
			}

			for (std::unique_ptr<FbxObject3D>& object1 : object)
			{
				if (object1->GetFileName() == "enemy")
				{

					//����ł����珜�O
					if (enemy->GetIsDead(object1->GetEnemyNum())) {
						break;
					}

					//�������m�Ƃ���������g�ݍ��킹�͏��O
					if (object0->GetEnemyNum() == object1->GetEnemyNum() || (object0->GetEnemyNum() < object1->GetEnemyNum())) {
						break;
					}

					//�������Ă�����
					if (ColliderManager::CheckCollider(object0->GetColliderData(), object1->GetColliderData()))
					{
						enemy->OnCollisionToEnemy(object0->GetEnemyNum(), enemy->GetPosition((object1->GetEnemyNum())));
						enemy->OnCollisionToEnemy(object1->GetEnemyNum(), enemy->GetPosition((object0->GetEnemyNum())));
					}
				}

			}
		}
	}

	//�㏈��
	ColliderManager::PostUpdate();
}

void GameScene::UpdateSprite()
{
	//�V�[���I��
	if (serectScene) {
		titleSerectButtonSprite->SetColor({ 1,1,1 });
		stageSerectButtonSprite->SetColor({ 1,0,0 });
	}
	else {
		titleSerectButtonSprite->SetColor({ 1,0,0 });
		stageSerectButtonSprite->SetColor({ 1,1,1 });
	}

	//�X�e�[�W�I��
	switch (serectStage) {
	case 0:
		stage1Sprite->SetColor({ 1,0,0 });
		stage2Sprite->SetColor({ 1,1,1 });
		stage3Sprite->SetColor({ 1,1,1 });
		break;

	case 1:
		stage1Sprite->SetColor({ 1,1,1 });
		stage2Sprite->SetColor({ 1,0,0 });
		stage3Sprite->SetColor({ 1,1,1 });
		break;

	case 2:
		stage1Sprite->SetColor({ 1,1,1 });
		stage2Sprite->SetColor({ 1,1,1 });
		stage3Sprite->SetColor({ 1,0,0 });
		break;
	}

	switch (scene) {
	case TITLE:
		titleSprite->Update();
		break;
	case SERECT:
		stageSelectSprite->Update();
		stage1Sprite->Update();
		stage2Sprite->Update();
		stage3Sprite->Update();
		break;
	case PLAY:
		menuButtonSprite->Update();
		break;
	case MENU:
		menuSprite->Update();
		menuCloseSprite->Update();
		titleSerectButtonSprite->Update();
		stageSerectButtonSprite->Update();
		break;
	case GAMEOVER:
		gameoverSprite->Update();
		titleSerectButtonSprite->Update();
		stageSerectButtonSprite->Update();
		break;
	case CLEAR:
		clearSprite->Update();
		titleSerectButtonSprite->Update();
		stageSerectButtonSprite->Update();
		break;
	}
}

void GameScene::SceneChange()
{
	switch (scene) {
	case TITLE:
		//�X�y�[�X�ŃX�e�[�W�Z���N�g
		if (input_->TriggerKey(DIK_SPACE)) {
			scene = SERECT;
		}
		break;
	case SERECT:
		//���E�L�[�ŃX�e�[�W�Z���N�g�A�X�y�[�X�Ō���
		if (input_->TriggerKey(DIK_RIGHT) || input_->TriggerKey(DIK_D)) {
			if (serectStage < 3) {
				serectStage++;
			}
		}
		else if (input_->TriggerKey(DIK_LEFT) || input_->TriggerKey(DIK_A)) {
			if (serectStage > 0) {
				serectStage--;
			}
		}
		if (input_->TriggerKey(DIK_SPACE)) {
			scene = PLAY;
		}
		break;
	case PLAY:
		//�v���C���[���S�ŃQ�[���I�[�o�[
		if (player->GetIsDead()) {
			scene = GAMEOVER;
		}
		//�X�e�[�W�N���A�ŃN���A
		else if (isClear) {
			scene = CLEAR;
		}
		else if (input_->TriggerKey(DIK_M)) {
			scene = MENU;
		}

		if (input_->TriggerKey(DIK_C)) {
			isClear = true;
		}

		break;
	case MENU:
		//���E�L�[�ŃV�[���Z���N�g�A�X�y�[�X�Ō���
		if (input_->TriggerKey(DIK_RIGHT) || input_->TriggerKey(DIK_D)) {
			serectScene = 1;
		}
		else if (input_->TriggerKey(DIK_LEFT) || input_->TriggerKey(DIK_A)) {
			serectScene = 0;
		}

		if (input_->TriggerKey(DIK_SPACE)) {

			if (serectScene) {
				scene = SERECT;
			}
			else {
				scene = TITLE;
			}

			//�v�f���Z�b�g
			ResetSceneData();

		}
		//M�L�[�Ńv���C�V�[���ɖ߂�
		else if (input_->TriggerKey(DIK_M)) {
			scene = PLAY;
		}



	case GAMEOVER:
		//���E�L�[�ŃV�[���Z���N�g�A�X�y�[�X�Ō���
		if (input_->TriggerKey(DIK_RIGHT) || input_->TriggerKey(DIK_D)) {
			serectScene = 1;
		}
		else if (input_->TriggerKey(DIK_LEFT) || input_->TriggerKey(DIK_A)) {
			serectScene = 0;
		}
		if (input_->TriggerKey(DIK_SPACE)) {

			if (serectScene) {
				scene = SERECT;
			}
			else {
				scene = TITLE;
			}

			//�v�f���Z�b�g
			ResetSceneData();

		}
		break;

	case CLEAR:
		//���E�L�[�ŃV�[���Z���N�g�A�X�y�[�X�Ō���
		if (input_->TriggerKey(DIK_RIGHT) || input_->TriggerKey(DIK_D)) {
			serectScene = 1;
		}
		else if (input_->TriggerKey(DIK_LEFT) || input_->TriggerKey(DIK_A)) {
			serectScene = 0;
		}
		if (input_->TriggerKey(DIK_SPACE)) {

			if (serectScene) {
				scene = SERECT;
			}
			else {
				scene = TITLE;
			}

			//�v�f���Z�b�g
			ResetSceneData();
		}
		break;
	}

}

void GameScene::Draw()
{
	//ImGui
	ImGui::Begin("GameScene");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(500, 150));
	ImGui::InputInt("DrawFbx", drawFbx);
	ImGui::InputInt("DrawSprite", drawSprite);
	ImGui::InputInt("DrawCollider", drawCollider);
	ImGui::InputInt("DrawParticle", drawParticle);
	ImGui::End();

	//�R���C�_�[�̕`��
	if (*drawCollider == 1)DrawCollider();
	//�X�v���C�g�̕`��
	if (*drawSprite == 1)DrawSprite();
	//FBX�̕`��
	if (*drawFbx == 1)DrawFBX();
	//�p�[�e�B�N���̕`��
	if (*drawParticle == 1)DrawParticle();

	/*billboardSprite->Draw(dxCommon_->GetCommandList());*/
}

void GameScene::DrawFBXLightView()
{
	enemy->DrawLightView(dxCommon_->GetCommandList());
	player->DrawLightView(dxCommon_->GetCommandList());

	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->DrawLightView(dxCommon_->GetCommandList());
	}
}

void GameScene::DrawFBX()
{
	enemy->Draw(dxCommon_->GetCommandList());
	player->Draw(dxCommon_->GetCommandList());

	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->Draw(dxCommon_->GetCommandList());
	}
}

void GameScene::DrawCollider()
{
	ColliderManager::Draw(dxCommon_->GetCommandList());
}

void GameScene::DrawSprite()
{
	switch (scene) {
	case TITLE:
		titleSprite->Draw(dxCommon_->GetCommandList());
		break;
	case SERECT:
		stageSelectSprite->Draw(dxCommon_->GetCommandList());
		stage1Sprite->Draw(dxCommon_->GetCommandList());
		stage2Sprite->Draw(dxCommon_->GetCommandList());
		stage3Sprite->Draw(dxCommon_->GetCommandList());
		break;
	case PLAY:
		menuButtonSprite->Draw(dxCommon_->GetCommandList());
		break;
	case MENU:
		menuSprite->Draw(dxCommon_->GetCommandList());
		menuCloseSprite->Draw(dxCommon_->GetCommandList());
		titleSerectButtonSprite->Draw(dxCommon_->GetCommandList());
		stageSerectButtonSprite->Draw(dxCommon_->GetCommandList());
		break;
	case GAMEOVER:
		gameoverSprite->Draw(dxCommon_->GetCommandList());
		titleSerectButtonSprite->Draw(dxCommon_->GetCommandList());
		stageSerectButtonSprite->Draw(dxCommon_->GetCommandList());
		break;
	case CLEAR:
		clearSprite->Draw(dxCommon_->GetCommandList());
		titleSerectButtonSprite->Draw(dxCommon_->GetCommandList());
		stageSerectButtonSprite->Draw(dxCommon_->GetCommandList());
		break;
	}
}

void GameScene::DrawParticle()
{
	sparkParticle->Draw(dxCommon_->GetCommandList());
	sparkParticle2->Draw(dxCommon_->GetCommandList());
	explosionParticle1->Draw(dxCommon_->GetCommandList());
	explosionParticle2->Draw(dxCommon_->GetCommandList());
}

void GameScene::ResetSceneData()
{
	isClear = false;
	serectStage = 0;
	serectStage = 0;

	player->Reset();
}

void GameScene::SetSRV(ID3D12DescriptorHeap* SRV)
{
	player->SetSRV(SRV);
	enemy->SetSRV(SRV);

	for (std::unique_ptr<FbxObject3D>& object0 : object)
	{
		object0->SetSRV(SRV);
	}
}

DirectX::XMMATRIX GameScene::GetLightViewProjection()
{
	return light->GetMatViewProjection();
}
