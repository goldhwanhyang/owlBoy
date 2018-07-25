#pragma once
#include "enemy.h"
#include "bullet.h"
#define MAX_STATE 7

class boss1 : public enemy
{
private:
	enum STATE
	{
		READY,
		TURN,
		WALK,
		WALK_SHINING,
		ATTACK,
		TAKE_SHIELD,
		OFF_SHIELD,
		OFF_WALK
	};
	float _speed2;
	bool _isShield;
	RECT _shieldHitBox;
	bool _isAttack;
	bullet* _bullet;

	image* _boss1Image[MAX_STATE];

	char _debug[128];
public:
	virtual void attack();
	virtual void move();

	virtual HRESULT init(float x, float y);
	virtual void update();
	virtual void render();
	virtual void release();

	bool frameMake(image * bmp, int & count, int & index, int frameY1, int frameY2, int cooltime, bool renderDir);

	boss1() {}
	~boss1() {}
};

