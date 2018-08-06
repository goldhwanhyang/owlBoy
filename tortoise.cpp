#include "stdafx.h"
#include "tortoise.h"
#include "player.h"

HRESULT tortoise::init(float x, float y)
{	//CHECK 이미지
	//==========추가 이미지
	IGM->addFrameImage("거북이_페이즈1_레디", "Texture/Enemies/Boss1/bossReady_975x246_5x1.bmp", 975, 246, 5, 1);
	IGM->addFrameImage("거북이_죽음", "Texture/Enemies/Boss1/bossDead_309X504_1x2.bmp", 309, 504, 1, 2);
	IGM->addFrameImage("거북이_불릿폭발이펙트", "Texture/Enemies/Boss1/bossBulletEffect_468x111_6x1.bmp", 468, 111, 6, 1);
	EFFECTMANAGER->addEffect("거북이_불릿폭발", "거북이_불릿폭발이펙트", 0.2, 40);
	//====================
	_phase1 = new tortoisePhase1;
	_phase1->init(x, y);
	_phase1->setPlayerLink(_player);
	_phase1->setShieldLink(_shield);
	
	_phase2 = new tortoisePhase2;
	_currentPhase = _phase1;
	_isPhase2 = false;

	_isActive = true;
	_deadCount = 0;

	return S_OK;
}

void tortoise::update()
{
	if (_isActive)
	{
		if (_currentPhase->getIsActive())
		{
			_currentPhase->update();
		}
		else
		{
			if (!_isPhase2)
			{
				//1페이즈가 끝나면 방패까지 보스가 걸어간뒤에 방패를 줍고 그뒤에 2페이즈 레디(점프하고) 2페이즈 시작
				//TODO : 플레이어를 멈추고 방패까지 걸어간뒤 2페이즈 시작
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
				//TODO : 클리어 - 죽음이펙트 플레이
				//EFFECTMANAGER->play("거북이_죽음폭발", _phase2->getX(), _phase2->getY());
				//TODO : 보스가 죽으면 혹시 tortoise를 릴리즈를 해버릴수 있을까?
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
		}
		else if (_isPhase2) //활성화 false이고 페이즈2이면
		{
			IMAGEMANAGER->frameRender("거북이_죽음", getMemDC(), _phase2->getX() - 135 - CAM->getX(), _phase2->getY() - 110 - CAM->getY(), 0, _phase2->getDir());
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
}
