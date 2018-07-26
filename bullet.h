#pragma once
#include "actor.h"
class bullet : public actor
{
	image* _image;	// �Ҹ��̹���
	float _radius;	// �Ҹ� ������
	float _range;	// �Ҹ���Ÿ�

	float _fireX, _fireY;
	
	bool _isActive;
	
	char _debug[64];
public:
	virtual HRESULT init(float radius, float speed, float range, const char* imageName = "����");
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

