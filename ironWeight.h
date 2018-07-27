#pragma once
#include "stuff.h"

#define MAX_WEIGHT_STATE 4

class player;

class ironWeight :
	public stuff
{

	image * _img[MAX_WEIGHT_STATE];

public:
	HRESULT init();
	void release();
	void update();
	void render();

	virtual void trampled(float weight) {}
	//virtual void lifted(player* _player);
	virtual void move();
	virtual void throwed(float speed, float angle);
	virtual void collide();

	ironWeight() :stuff() { _isLiftable = true; _weight = 100; }
	~ironWeight() {}
};

