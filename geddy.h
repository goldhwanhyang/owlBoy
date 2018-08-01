#pragma once
#include "liftableActor.h"


class geddy : public liftableActor
{
	enum {IDLE, HANG, ON_AIR, };
private:
	image * _img;
public:
	virtual void attack();
	virtual void damaged(actor *e);	// ü�� ����, �˹�, ���� ����Ʈ����(������), ������ �� �ǰݽ� �����(�Ե�, ������)
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

