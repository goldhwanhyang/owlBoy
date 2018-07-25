#pragma once
#include "enemy.h"
#include "bullet.h"
class boss1 : public enemy
{
private:
	enum STATE1
	{
		READY,
		WALK,
		ATTACK,
		TAKE_SHIELD,
		OFF_SHIELD,
		OFF_WALK
	};
	STATE1 _state1;
	float _speed2;
	bool _isShield;
	RECT _shieldHitBox;
	bool _isAttack;
	bullet* _bullet;

	char _debug[128];
public:
	virtual void attack();
	virtual void move();

	virtual HRESULT init();
	virtual HRESULT init(float x, float y);
	virtual void update();
	virtual void render();
	virtual void release();

	boss1() {}
	~boss1() {}
};

