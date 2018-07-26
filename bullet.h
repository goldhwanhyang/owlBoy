#pragma once
#include "actor.h"
class bullet : public actor
{
	image* _image;	// �Ҹ��̹���
	int _bulletMax; // �Ҹ�����
	float _range;	// �Ҹ���Ÿ�

	vector<bullet*> _vBullet;
	vector<bullet*>::iterator _viBullet;

public:
	virtual HRESULT init(const char* imageName, int bulletMax, float range);
	virtual void update();
	virtual void render();
	virtual void release();

	void fire();
	virtual void move();
	virtual void collide();
	vector<bullet*> getvBullet() { return _vBullet; }

	bullet() {}
	~bullet() {}
};

