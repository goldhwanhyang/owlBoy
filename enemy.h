#pragma once
#include "actor.h"
class enemy : public actor
{
public:
	virtual HRESULT init();
	virtual void update();
	virtual void render();
	virtual void release();

	enemy() {}
	~enemy() {}
};

