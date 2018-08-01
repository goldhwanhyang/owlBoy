#pragma once
#include "actor.h"
#include "image.h"

class player;

#define LIFT_TIME 20

enum STUFF_STATE
{
	BURIED, ON_GROUND, HANG, ON_AIR
};

class liftableActor : public actor
{

protected:
	int _maxWidth, _maxHeight;
	int _height;
	image* _mapPixel;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	virtual void attack() {}
	virtual void damaged(actor *e) {}
	virtual void trampled(float weight) {}
	//virtual void used(player* _player) {}
	virtual void lifted(player* _player);
	virtual void collide();
	virtual void move();
	virtual void throwed(float speed, float angle);

	liftableActor() : actor() {}
	~liftableActor() {}
};

