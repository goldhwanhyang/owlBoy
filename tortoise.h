#pragma once
#include "enemy.h"
#include "tortoisePhase1.h"
#include "tortoisePhase2.h"
#include "tortoiseShield.h"

class tortoise : public enemy
{
	enemy* _currentPhase;
	tortoisePhase1* _phase1;
	tortoisePhase2* _phase2;
	tortoiseShield* _shield;			//πÊ∆–

	bool _isPhase2;
	bool _isActiveShield;

	int _deadCount;

	image* _hpBarHead;
	progressBar* _hpBar;
public:
	virtual HRESULT init(float x, float y);
	virtual void update();
	virtual void render();
	virtual void release();
	virtual void damaged(actor *e);

	virtual void setShieldLink(tortoiseShield* shield) { _shield = shield; }

	tortoise() {}
	~tortoise() {}
};

