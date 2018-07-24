#pragma once
#include "enemy.h"
class boss1 : public enemy
{

public:
	virtual HRESULT init();
	virtual void update();
	virtual void render();
	virtual void release();

	boss1() {}
	~boss1() {}
};

