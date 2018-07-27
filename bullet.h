#pragma once
#include "actor.h"
class bullet : public actor
{
	image* _image;		// �Ҹ��̹���
	image* _pixelMap;	// �ȼ��� �޾ƿ���
	float _radius;		// �Ҹ� ������
	float _range;		// �Ҹ���Ÿ�

	float _fireX, _fireY;
	
	bool _isActive;
public:
	virtual HRESULT init(float radius, float speed, float range, const char* imageName = "����");
	virtual void update();
	virtual void render();
	virtual void release();

	virtual void collide(string pixelImageName); //������ �浹�� �ϱ����� �ȼ��̹���Ű���� �Ѱ��ְ� ����

	void setPixelMap(image* pixelMap) { _pixelMap = pixelMap; }
	float getRange() { return _range; }
	bool getIsActive() { return _isActive; }

	void setIsActive(bool isActive) { _isActive = isActive; }
	void setFireCenter(float fireX, float fireY) { _fireX = fireX; _fireY = fireY; _x = fireX; _y = fireY; }

	bullet() {}
	~bullet() {}
};

