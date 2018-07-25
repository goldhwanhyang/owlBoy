#pragma once
#include "enemy.h"
class gawk : public enemy
{
public:
	virtual HRESULT init(float x, float y);
	virtual void update();
	virtual void render();
	virtual void release();

	virtual void move();

	gawk() {}
	~gawk() {}
};

