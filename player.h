#pragma once
#include "actor.h"

#define OTUS_WIDTH 30
#define OTUS_HEIGTH 50

#define STATE_MAX 4
enum STATE { IDLE , WALK, JUMP , FLY };

class player : public actor
{
private:
	image* img;
	image* img_2[STATE_MAX];
	image* _testMap;

	STATE _state;
	bool isDrop;
public:
	HRESULT init();
	void update();
	void render();
	void release();

	player(){}
	~player(){}
};