#pragma once
#include "enemy.h"
class boss1 : public enemy
{
	RECT _shieldHitBox;
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

