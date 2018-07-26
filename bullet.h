#pragma once
#include "actor.h"
class bullet : public actor
{
	image* _image;	// 불릿이미지
	float _radius;	// 불릿 반지름
	float _range;	// 불릿사거리

	float _fireX, _fireY;
	
	bool _isActive;
	
	char _debug[64];
public:
	virtual HRESULT init(float radius, float speed, float range, const char* imageName = "없음");
	virtual void update();
	virtual void render();
	virtual void release();

	virtual void collide();

	float getRange() { return _range; }
	bool getIsActive() { return _isActive; }

	void setIsActive(bool isActive) { _isActive = isActive; }
	void setFireLocationXY(float fireX, float fireY) { _fireX = fireX; _fireY = fireY; _x = fireX; _y = fireY; }

	bullet() {}
	~bullet() {}
};

