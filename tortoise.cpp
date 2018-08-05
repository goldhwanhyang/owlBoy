#include "stdafx.h"
#include "tortoise.h"
#include "player.h"

HRESULT tortoise::init(float x, float y)
{	//==========�߰� �̹���
	IGM->addFrameImage("�ź���_������1_����", "Texture/Enemies/Boss1/bossReady_975x246_5x1.bmp", 975, 246, 5, 1);
	IGM->addFrameImage("�ź���_����", "Texture/Enemies/Boss1/bossDead_309X504_1x2.bmp", 309, 504, 1, 2);
	IGM->addFrameImage("�ź���_�Ҹ���������Ʈ", "Texture/Enemies/Boss1/bossBulletEffect_468x111_6x1.bmp", 468, 111, 6, 1);
	EFFECTMANAGER->addEffect("�ź���_�Ҹ�����", "�ź���_�Ҹ���������Ʈ", 0.2, 40);
	//====================
	_phase1 = new tortoisePhase1;
	_phase1->init(x, y);
	_phase1->setPlayerLink(_player);
	_phase1->setShieldLink(_shield);
	
	_phase2 = new tortoisePhase2;
	_currentPhase = _phase1;
	_isPhase2 = false;

	return S_OK;
}

void tortoise::update()
{
	if (_currentPhase->getHp() > 0)
	{
		_currentPhase->update();
	}
	else
	{
		if (!_isPhase2)
		{
			//1����� ������ �ƾ����� ���б��� ������ �ɾ�ڿ� ���и� �ݰ� �׵ڿ� 2������ ����(�����ϰ�) 2������ ����
			_shield->setIsActive(true);
			_phase2->init(_phase1->getX(), _phase1->getY(), _phase1->getDir());
			_phase2->setPlayerLink(_player);
			_phase2->setShieldLink(_shield);
			_currentPhase = _phase2;
			_currentPhase->setHp(100);
			_isPhase2 = true;
		}
		else
		{
			//TODO : Ŭ���� - ��������Ʈ ������Ʈ����
			//TODO : ��������Ʈ ������ ������ ������Ʈ�� ���� �Ѵ� ������ (Ȥ�� ����� �ع����� ������?)
		}
	}
}

void tortoise::render()
{
	if (_currentPhase->getHp() > 0)
	{
		_currentPhase->render();
	}
	else if (_isPhase2) //ü���� 0�̰� ������2�̸�
	{
		IMAGEMANAGER->frameRender("�ź���_����", getMemDC(), _phase2->getX() - 135 - CAM->getX(), _phase2->getY() - 110 - CAM->getY(), 0, _phase2->getDir());
		//TODO : ��������Ʈ ����
	}
}

void tortoise::release()
{
	_phase1->release();
	SAFE_DELETE(_phase1);
	_phase2->release();
	SAFE_DELETE(_phase2);
}
