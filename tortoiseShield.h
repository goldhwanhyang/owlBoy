#pragma once
#include "enemy.h"
namespace SHIELD_CONST
{
	const int HITBOX_WIDTH = 220;
	const int HITBOX_HEIGHT = 240;
}
class tortoiseShield : public enemy
{
private:
	image* _image;					//떨어진 방패 이미지
	bool _isActive;					//방패 챙겼니?
	bool _onGround;					//땅에 떨어진 뒤에 중력이 계속 작용해서 통통튀기는걸 막음

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

