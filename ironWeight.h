#pragma once
#include "liftableActor.h"

#define MAX_WEIGHT_STATE 4

class player;

class ironWeight :
	public liftableActor
{
	image * _img[MAX_WEIGHT_STATE];

public:
	HRESULT init();
	void release();
	void update();
	void render();

	virtual void trampled(float weight) {}

	ironWeight() :liftableActor() { _isLiftable = true; _weight = 100; }
	~ironWeight() {}
};

