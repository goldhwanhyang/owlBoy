#pragma once
#include "actor.h"
class stuff : public actor
{
protected:

public:
	HRESULT init();
	void release();
	void update();
	void render();

	virtual void trampled();

	stuff();
	~stuff();
};

