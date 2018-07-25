#pragma once
#include "actor.h"
class bullet : public actor
{
public:
	virtual HRESULT init();
	virtual void update();
	virtual void render();
	virtual void release();

	bullet() {}
	~bullet() {}
};

