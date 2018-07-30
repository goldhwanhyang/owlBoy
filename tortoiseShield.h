#pragma once
#include "enemy.h"
class tortoiseShield : public enemy
{
	image* _image;					//¶³¾îÁø ¹æÆÐ ÀÌ¹ÌÁö
	bool _isActive;					//¹æÆÐ Ã¬°å´Ï?

	char _debug[64];
public:
	virtual void damaged(actor *e);
	virtual void move();
	virtual void collide();

	HRESULT init(float x, float y);
	void update();
	void render();
	void release();

	bool getIsActive() { return _isActive; }
	void setIsActive(bool isActive) { _isActive = isActive; }

	int getWidth() { return _image->getFrameWidth(); }
	int getHeight() { return _image->getFrameHeight(); }

	tortoiseShield() {}
	~tortoiseShield() {}
};

