#pragma once
#include "actor.h"

#define OTUS_WIDTH 50
#define OTUS_HEIGTH 125
#define PI 3.14
#define STATE_MAX 7
enum STATE { IDLE , WALK, JUMP, JUMPFALL , FLY , FLYDOWN, ROLL };

class player : public actor
{
private:
	STATE _state;
	STATE _beforeState;


	image* img[STATE_MAX];
	image* _mapPixel;
	image* _map;

	bool _isLeft;
	bool _isWalk;
	bool _isJump;
	bool _isFly;
	bool _isFlyDown;
	int _directionX;
	int _directionY;
	int _jumpCount;

public:
	HRESULT init();
	void update();
	void render();
	void release();

	void setMap(image *map) { _map = map; }
	void setMapPixel(image *mapPixel) { _mapPixel = mapPixel; }
	void collide();
	void inputKey();
	void frameSetting();
	player(){}
	~player(){}
};