#pragma once
#include "actor.h"
class ring :
	public actor
{
	image * frontRing;
	image * backRing;


public:
	virtual void damaged(actor *e);	// ü�� ����, �˹�, ���� ����Ʈ����(������), ������ �� �ǰݽ� �����(�Ե�, ������)
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

