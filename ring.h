#pragma once
#include "actor.h"
class ring :
	public actor
{
	image * frontRing;
	image * backRing;


public:
	virtual void damaged(actor *e);	// 체력 감소, 넉백, 동료 떨어트리기(오투스), 서있을 때 피격시 사라짐(게디, 알폰스)
	virtual void collide();
	virtual void drawUI() {}
	

	virtual HRESULT init(float x, float y, int dir);
	virtual HRESULT init();
	virtual void update();
	virtual void render();
	void renderFront();
	void renderBack();
	virtual void release();

	ring() {}
	~ring() {}
};

