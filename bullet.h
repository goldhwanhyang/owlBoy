#pragma once
#include "actor.h"
class bullet : public actor
{
	image* _image;		// 불릿이미지
	image* _pixelMap;	// 픽셀맵 받아오기
	float _radius;		// 불릿 반지름
	float _range;		// 불릿사거리

	float _fireX, _fireY;
	
	bool _isActive;
public:
	virtual HRESULT init(float radius, float speed, float range, const char* imageName = "없음");
	virtual void update();
	virtual void render();
	virtual void release();

	virtual void collide(string pixelImageName); //벽과의 충돌을 하기위해 픽셀이미지키값을 넘겨주게 했음

	void setPixelMap(image* pixelMap) { _pixelMap = pixelMap; }
	float getRange() { return _range; }
	bool getIsActive() { return _isActive; }

	void setIsActive(bool isActive) { _isActive = isActive; }
	void setFireCenter(float fireX, float fireY) { _fireX = fireX; _fireY = fireY; _x = fireX; _y = fireY; }

	bullet() {}
	~bullet() {}
};

