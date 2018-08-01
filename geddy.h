#pragma once
#include "liftableActor.h"


class geddy : public liftableActor
{
	enum {IDLE, HANG, ON_AIR, };
private:
	image * _img;
public:
	virtual void attack();
	virtual void damaged(actor *e);	// 체력 감소, 넉백, 동료 떨어트리기(오투스), 서있을 때 피격시 사라짐(게디, 알폰스)
	virtual void move();
	virtual void collide();
	//virtual void throwed(float speed, float angle);
	virtual void lifted(player* _player);
	virtual void drawUI();

	virtual HRESULT init();
	virtual void update();
	virtual void render();
	virtual void release();

	geddy() {}
	~geddy() {}
};

