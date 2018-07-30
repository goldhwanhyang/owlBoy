#include "stdafx.h"
#include "tortoise.h"
#include "player.h"

HRESULT tortoise::init(float x, float y)
{
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
	_currentPhase->update();
	if (_currentPhase->getHp() == 0)
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
			//TODO : Ŭ����
		}
	}
}

void tortoise::render()
{
	_currentPhase->render();
}

void tortoise::release()
{
	_phase1->release();
	SAFE_DELETE(_phase1);
	_phase2->release();
	SAFE_DELETE(_phase2);
}
