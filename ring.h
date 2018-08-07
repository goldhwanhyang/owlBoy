#pragma once
#include "actor.h"

class player;

class ring :
	public actor
{
	image * frontRing;
	image * backRing;

	float _dirY;
	int _alpha;

public:
	void damaged(player *e);	// ü�� ����, �˹�, ���� ����Ʈ����(������), ������ �� �ǰݽ� �����(�Ե�, ������)
	virtual void collide();
	bool collide(player * a);
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

