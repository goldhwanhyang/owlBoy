#include "stdafx.h"
#include "tortoise.h"
#include "player.h"

HRESULT tortoise::init(float x, float y)
{	//CHECK �̹���
	//==========�߰� �̹���
	IGM->addFrameImage("�ź���_������1_����", "Texture/Enemies/Boss1/bossReady_975x246_5x1.bmp", 975, 246, 5, 1);
	IGM->addFrameImage("�ź���_����", "Texture/Enemies/Boss1/bossDead_309X504_1x2.bmp", 309, 504, 1, 2);
	IGM->addFrameImage("�ź���_�Ҹ���������Ʈ", "Texture/Enemies/Boss1/bossBulletEffect_468x111_6x1.bmp", 468, 111, 6, 1);
	EFFECTMANAGER->addEffect("�ź���_�Ҹ�����", "�ź���_�Ҹ���������Ʈ", 0.2, 40);
	IGM->addImage("�ź���_ü�¹ٸӸ�", "Texture/Enemies/Boss1/bossHpBarHead.bmp", 54, 69, true, MAGENTA);
	//====================
	_phase1 = new tortoisePhase1;
	_phase1->init(x, y);
	_phase1->setPlayerLink(_player);
	_phase1->setMapPixel(_mapPixel);
	_phase1->setShieldLink(_shield);
	
	_phase2 = new tortoisePhase2;
	_currentPhase = _phase1;
	_isPhase2 = false;

	_isActive = true;
	_deadCount = 0;

	_hpBarHead = IMAGEMANAGER->findImage("�ź���_ü�¹ٸӸ�");
	_hpBar = new progressBar;
	_hpBar->init("Texture/Enemies/Boss1/hpBarFront", "Texture/Enemies/Boss1/hpBarBack", WINSIZEX*0.68, WINSIZEY*0.05, 300, 51);
	_hpBar->setGauge(_currentPhase->getHp(), _currentPhase->getMaxHp());

	return S_OK;
}

void tortoise::update()
{
	if (_isActive)
	{
		if (_currentPhase->getIsActive())
		{
			_currentPhase->update();
			//hp�� ������Ʈ
			_hpBar->update();
			_hpBar->setGauge(_currentPhase->getHp(), _currentPhase->getMaxHp());
		}
		else
		{
			if (!_isPhase2)
			{
				//1����� ������ ���б��� ������ �ɾ�ڿ� ���и� �ݰ� �׵ڿ� 2������ ����(�����ϰ�) 2������ ����
				//TODO : �÷��̾ ���߰� ���б��� �ɾ�� 2������ ����
				_shield->setIsActive(true);
				_phase2->init(_phase1->getX(), _phase1->getY(), _phase1->getDir());
				_phase2->setPlayerLink(_player);
				_phase2->setMapPixel(_mapPixel);
				_phase2->setShieldLink(_shield);
				_currentPhase = _phase2;
				_currentPhase->setHp(100);
				_isPhase2 = true;
			}
			else
			{
				//TODO : Ŭ���� - ��������Ʈ �÷���
				//EFFECTMANAGER->play("�ź���_��������", _phase2->getX(), _phase2->getY());
				++_deadCount;
			}
		}
	}
}

void tortoise::render()
{
	if (_isActive)
	{
		if (_currentPhase->getIsActive())
		{
			_currentPhase->render();
			//hpBar����
			if (_player->getX() < 2000)
			{
				_hpBarHead->render(getMemDC(), _hpBar->getRect().left - _hpBarHead->getWidth(),_hpBar->getRect().top-7);
				_hpBar->render();
			}
		}
		else if (_isPhase2) //Ȱ��ȭ false�̰� ������2�̸�
		{
			IMAGEMANAGER->frameRender("�ź���_����", getMemDC(), _phase2->getX() - 135 - CAM->getX(), _phase2->getY() - 110 - CAM->getY(), 0, _phase2->getDir());
			if (_deadCount > 50)
			{
				_isActive = false;
			}
		}
	}
}

void tortoise::release()
{
	_phase1->release();
	SAFE_DELETE(_phase1);
	_phase2->release();
	SAFE_DELETE(_phase2);
	_hpBar->release();
	SAFE_DELETE(_hpBar);
}
