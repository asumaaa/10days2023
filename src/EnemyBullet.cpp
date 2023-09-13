#include "EnemyBullet.h"
#include "mathOriginal.h"
#include "ColliderManager.h"

Camera* EnemyBullet::camera = nullptr;
Input* EnemyBullet::input = nullptr;
DXInput* EnemyBullet::dxInput = nullptr;
FbxModel* EnemyBullet::model = nullptr;

void EnemyBullet::Initialize()
{
}

void EnemyBullet::Update()
{
	//�e����������
	DeleteBullet();

	//�V���b�g�t���O����������e����
	//if (shotFlag)
	//{
	//	CreateBullet();
	//}

	//����
	Move();

	//�^�C�}�[���Z
	for (int i = 0; i < object.size(); i++)
	{
		timer[i] += 1.0f;
	}

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

	//�V���b�g�t���O���U��
	shotFlag = false;
}

void EnemyBullet::Draw(ID3D12GraphicsCommandList* cmdList)
{
	for (std::unique_ptr<FbxObject3D>& objects : object)
	{
		objects->Draw(cmdList);
	}
}

void EnemyBullet::DrawLightView(ID3D12GraphicsCommandList* cmdList)
{
	for (std::unique_ptr<FbxObject3D>& objects : object)
	{
		objects->DrawLightView(cmdList);
	}
}

void EnemyBullet::Move()
{
	if (object.size() != 0)
	{
		for (int i = 0; i < object.size(); i++)
		{
			//�i�s�x�N�g�������Z
			position[i] = position[i] + (velocity[i] * posSpeed);
		}
	}
}

void EnemyBullet::CreateBullet()
{
	//�I�u�W�F�N�g����
	std::unique_ptr<FbxObject3D>newObject = std::make_unique<FbxObject3D>();
	newObject->Initialize();
	newObject->SetModel(model);

	//�I�u�W�F�N�g���Ƃɖ��O������
	std::string objectName = "EnemyBullet" + std::to_string(number);

	//�I�u�W�F�N�g�f�[�^�Z�b�g
	JSONLoader::ObjectData objectData;
	objectData.fileName = "EnemyBullet";
	objectData.objectName = objectName;
	objectData.position = position.back();
	objectData.rotation = rotation.back();
	objectData.scale = scale.back();
	newObject->SetObjectData(objectData);

	//�R���C�_�[�f�[�^�Z�b�g
	JSONLoader::ColliderData colliderData;
	colliderData.type = "Sphere";	//��������̂Ŏ�邽��
	colliderData.objectName = objectName;
	colliderData.scale = scale.back();
	colliderData.rotation = rotation.back();
	colliderData.center = position.back();
	newObject->SetColliderData(colliderData);

	//�R���C�_�[�}�l�[�W���[�ɃZ�b�g
	ColliderManager::SetCollider(colliderData);

	//�I�u�W�F�N�g�ɑ��
	object.emplace_back(std::move(newObject));

	number++;
}

void EnemyBullet::DeleteBullet()
{
	//�폜
	for (int i = 0; i < object.size(); i++)
	{
		//��莞�Ԍo������
		if (timer[i] >= destoryTime)
		{
			position.erase(position.begin() + i);
			rotation.erase(rotation.begin() + i);
			scale.erase(scale.begin() + i);
			velocity.erase(velocity.begin() + i);
			hitFlag.erase(hitFlag.begin() + i);
			timer.erase(timer.begin() + i);
			object.erase(std::next(object.begin(), i));

			continue;
		}

		//�q�b�g�t���O����������
		if (hitFlag[i] == true)
		{
			position.erase(position.begin() + i);
			rotation.erase(rotation.begin() + i);
			scale.erase(scale.begin() + i);
			velocity.erase(velocity.begin() + i);
			timer.erase(timer.begin() + i);
			hitFlag.erase(hitFlag.begin() + i);
			object.erase(std::next(object.begin(), i));
		}
	}
}

void EnemyBullet::SetSRV(ID3D12DescriptorHeap* SRV)
{
	for (std::unique_ptr<FbxObject3D>& objects : object)
	{
		objects->SetSRV(SRV);
	}
}

void EnemyBullet::SetBullet(XMFLOAT3 position, XMFLOAT3 velocity)
{
	EnemyBullet::position.emplace_back(position);
	EnemyBullet::rotation.emplace_back(baseRotation);
	EnemyBullet::scale.emplace_back(baseScale);
	EnemyBullet::velocity.emplace_back(velocity);
	EnemyBullet::timer.emplace_back(0.0f);
	EnemyBullet::hitFlag.emplace_back(false);
}

JSONLoader::ColliderData EnemyBullet::GetColliderData(int num)
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
