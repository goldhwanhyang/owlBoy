#pragma once
#include "liftableActor.h"
#include "bullet.h"

class player;

#define MAX_GEDDY_STATE 6
#define MAX_GEDDY_BULLET 20

#define GEDDY_WIDTH 50
#define GEDDY_HEIGHT 150

class geddy : public liftableActor
{
	enum {IDLE, ON_AIR, HANG, ATTACK, WALK, PREFARE};
private:
	image * _img[MAX_GEDDY_STATE];
	image * _handImg;

	bullet *_bullet;

	int _handsDir;
	int _shootingDir;
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

	int convertDir();

	geddy() {}
	~geddy() {}
};

