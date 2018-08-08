#pragma once
#include "liftableActor.h"

namespace SHIELD_CONST
{
	const int HITBOX_WIDTH = 220;
	const int HITBOX_HEIGHT = 240;
}
class tortoiseShield : public liftableActor
{
private:
	image* _image;					//������ ���� �̹���
	//bool _isActive;					//���� ì���?
	bool _onGround;					//���� ������ �ڿ� �߷��� ��� �ۿ��ؼ� ����Ƣ��°� ����

	char _debug[64];
public:
	virtual void damaged(actor *e);
	virtual void move();
	virtual void collide();
	virtual void lifted(player* _player);

	HRESULT init(float x, float y);
	void update();
	void render();
	void release();

	//bool getIsActive() { return _isActive; }
	void setIsActive(bool isActive) { _isActive = isActive; }

	int getWidth() { return _image->getFrameWidth(); }
	int getHeight() { return _image->getFrameHeight(); }

	tortoiseShield() {}
	~tortoiseShield() {}
};

