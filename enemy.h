#pragma once
#include "actor.h"
class enemy : public actor
{
protected:
	float _playerX, _playerY;
public:
	virtual HRESULT init(float x, float y);
	virtual void update();
	virtual void render();
	virtual void release();

	enemy() {}
	~enemy() {}
};

