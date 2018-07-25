#pragma once
#include "actor.h"
class bullet : public actor
{
	image* _image; //�Ҹ��̹���

public:
	virtual HRESULT init(const char* imageName, int bulletMax, float range);
	virtual void update();
	virtual void render();
	virtual void release();

	bullet() {}
	~bullet() {}
};

