#pragma once
#include "enemy.h"
class tortoiseShield : public enemy
{
	bool on;						//방패 챙겼니?
	float x;						//방패 x
	float y;						//방패 y
	RECT hitBox;					//방패 충돌박스
	int count;						//방패 프레임 카운트
	int index;						//방패 프레임 인덱스
	float offSpeed;					//방패가 없을때 속도
public:
	HRESULT init();
	void update();
	void render();
	void release();

	tortoiseShield() {}
	~tortoiseShield() {}
};

