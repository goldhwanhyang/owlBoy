#pragma once
#include "actor.h"
#include "image.h"

class player;

#define LIFT_TIME 20

enum STUFF_STATE
{
	ON_GROUND, ON_AIR, HANG, BURIED
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

	void setMapPixel(image* mapPixel) { _mapPixel = mapPixel; }

	virtual void attack() {}
	virtual liftableActor* use(actor *e) { return this; }
	virtual void damaged(actor *e) {}
	virtual void trampled(float weight) {}
	//virtual void used(player* _player) {}
	virtual void lifted(player* _player);
	virtual void collide();
	virtual bool collide(player * _player);
	virtual void move();
	virtual void throwed(float speed, float angle);

	liftableActor() : actor() {}
	~liftableActor() {}
};

