#pragma once
#include "actor.h"

class player : public actor
{
private:
	image* img;
	image* _testMap;
public:
	HRESULT init();
	void update();
	void render();
	void release();

	player(){}
	~player(){}
};

