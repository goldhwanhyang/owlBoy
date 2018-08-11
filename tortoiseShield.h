#pragma once
#include "liftableActor.h"

namespace SHIELD_CONST
{
	const int HITBOX_WIDTH = 220;
	const int HITBOX_HEIGHT = 160;
}
class tortoiseShield : public liftableActor
{
	enum {IDLE, HANG = 2};
private:
	image* _image;					//떨어진 방패 이미지
	image* _lifted;

	char _debug[64];
public:
	virtual void damaged(actor *e);
	virtual void move();
	virtual void collide();
	virtual void lifted(player* _player);
	virtual void throwed(float speed, float angle);

	HRESULT init(float x, float y);
	void update();
	void render();
	void release();

	int getWidth() { return _image->getFrameWidth(); }
	int getHeight() { return _image->getFrameHeight(); }

	tortoiseShield() {}
	~tortoiseShield() {}
};

