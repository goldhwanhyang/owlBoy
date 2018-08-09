#pragma once
#include "enemy.h"

namespace GAWK_CONST
{
	const int MAX_STATE = 6;
	const int HITBOX_WIDTH = 72;
	const int HITBOX_HEIGHT = 72;
}

class gawk : public enemy
{
private:
	image * _gawkImage[GAWK_CONST::MAX_STATE];
	enum STATE
	{
		IDLE,
		READY,
		FLY,
		FALL,
		STUN
	};
	enum DIR
	{
		RIGHT,
		LEFT
	};
	int _jumpDelay;
	int _delayCount;
	RECT _scanRc;
	float _shakeAngle;
	int _oldState;
public:
	virtual void damaged(actor* e);
	void stunShake();
	virtual void move();
	virtual void collide();

	void search();
	void turn();

	bool frameMake(image * bmp, int cooltime);

	virtual HRESULT init(float x, float y, int dir);
	virtual void update();
	virtual void render();
	virtual void release();

	gawk() {}
	~gawk() {}
};

