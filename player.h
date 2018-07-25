#pragma once
#include "actor.h"

#define OTUS_WIDTH 30
#define OTUS_HEIGTH 50
#define PI 3.14
#define STATE_MAX 4
enum STATE { IDLE , WALK, JUMP, JUMPFALL , FLY };

class player : public actor
{
private:
	//image* img;
	image* img[STATE_MAX];
	image* _testMap;
	float weight;
	STATE _state;
	bool _isLeft;
	bool _isWalk;
	bool _isJump;
public:
	HRESULT init();
	void update();
	void render();
	void release();

	player(){}
	~player(){}
};