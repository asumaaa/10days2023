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
private:	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	//�����o�֐�
public:
	//�ÓI�����o�֐�
	static void SetCamera(Camera* camera) { Enemy::camera = camera; }
	static void SetInput(Input* input) { Enemy::input = input; }
	static void SetDXInput(DXInput* dxInput) { Enemy::dxInput = dxInput; }

	//������
	void Initialize();
	//�X�V
	void Update(XMFLOAT3 playerPos);
	void UpdateObject();
	void UpdateSprite();
	//�`��
	void DrawSprite(ID3D12GraphicsCommandList* cmdList);
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);
	void SetSRV(ID3D12DescriptorHeap* SRV);

	//�����֘A
	//�����S��
	void TypeUpdate();

	//�ړ����
	void MoveHoming(int i);
	void MoveX(int i);
	void MoveZ(int i);

	//�ˌ�
	void Shot(int i,XMFLOAT3 velosity);
	void ShotX(int i);
	void ShotZ(int i);
	void ShotHoming(int i);

	//����
	void UpdateGravity();
	//�W�����v
	void UpdateJump();

	//�U���n�S��
	void UpdateAttack();

	//�Z�b�^�[
	void SetObject(FbxObject3D* object);
	void HitPlane();

	//void SetType(int num,int type) { this->type[num] = type; }
	void SetStageMid(XMFLOAT3 stageMid) { this->stageMid = stageMid; }
	void SetBullet(EnemyBullet* enemyBullet) { Enemy::bullet = enemyBullet; }

	//�Q�b�^�[
	XMFLOAT3 GetPosition(int num) { return position[num]; }
	XMFLOAT3 GetRotation(int num) { return rotation[num]; }
	XMFLOAT3 GetScale(int num) { return scale[num]; }

	//�ÓI�����o�ϐ�
private:
	//�J����
	static Camera* camera;
	//�L�[�{�[�h
	static Input* input;
	//�R���g���[���[
	static DXInput* dxInput;

	//�����o�ϐ�
public:

	//�I�u�W�F�N�g
	std::list<std::unique_ptr<FbxObject3D>>object;
	//�X�v���C�g
	std::unique_ptr<Sprite>spriteHpBar;

	//�ό`�s��
	//���W
	std::vector<XMFLOAT3> position;
	//��]
	std::vector<XMFLOAT3> rotation;
	//�T�C�Y
	std::vector<XMFLOAT3> scale;
	//���
	std::vector<int> type;

	//�v���C���[�̈ʒu
	XMFLOAT3 playerPosition = {};
	//�X�e�[�W�̒��S���W
	XMFLOAT3 stageMid = {};
	//�X�e�[�W�̃T�C�Y
	XMFLOAT3 stageSize = {40,1,40};

	//���� 0 = ��,�� 1 = �E,�� 
	std::vector<bool> moveX;
	std::vector<bool> moveZ;
	//�ړ��X�s�[�h
	float enemySpeed = 0.1;

	//�e
	EnemyBullet* bullet;
	float bulletSpeed = 0.2;
	//�ˌ�
	const int ShotCoolTime = 100;
	int shotCoolTimer = 0;
	//�ˌ��t���O
	bool isShot = false;

	//�����蔻��֘A
	//�ڒn�t���O
	bool groundFlag = false;


	//�����֘A
	
	//����
	//�����x�N�g��
	XMFLOAT3 fallVelocity = { 0.0f,0.0f,0.0f };
	//�����^�C�}�[
	float fallTimer = 0.0f;
	//�����ő�l�܂łɂ����鎞��
	float fallTime = 1.0f;
	//1�t���[��������̗�����
	float fallFrame = 1.0f / 60.0f;

	//�W�����v
	float jumpHeight = 0.4;

	//�X�s�[�h
	float speed = 0.15f;

	//HP
	float HP = 100;
};

