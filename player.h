#pragma once
#include "actor.h"

#define OTUS_WIDTH 50
#define OTUS_HEIGTH 120
#define PI 3.14
#define STATE_MAX 5
enum STATE { IDLE , WALK, JUMP, JUMPFALL , FLY };

class player : public actor
{
private:
	STATE _state;
	image* img[STATE_MAX];
	image* _TownMapPixel;
	image* _TownMap;

	float weight;
	bool _isLeft;
	bool _isWalk;
	bool _isJump;
	bool _isFly;
public:
	HRESULT init();
	void update();
	void render();
	void release();

	player(){}
	~player(){}
};