#pragma once
#include "backgroundActor.h"
class cloud :
	public backgroundActor
{
public:
	HRESULT init();
	void release();
	void update();
	void render();

	void changeImage();

	cloud() {}
	~cloud() {}
};

