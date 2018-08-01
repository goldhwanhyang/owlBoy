#pragma once
#include "enemy.h"

namespace GAWK_CONST
{
	const int MAX_STATE = 3;
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
		FLY,
		FALL,
		DAMAGED
	};
	enum DIR
	{
		RIGHT,
		LEFT
	};
	bool _isFall;
	int _delayCount;
	RECT _scanRc;
	float _shakeAngle;

	char _debug[64];
public:
	virtual void damaged(actor* e);
	virtual void move();

	void search();
	void turn();

	bool frameMake(image * bmp);

	virtual HRESULT init(float x, float y, int dir);
	virtual void update();
	virtual void render();
	virtual void release();

	gawk() {}
	~gawk() {}
};

