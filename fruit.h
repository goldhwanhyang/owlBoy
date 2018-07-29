#pragma once
#include "stuff.h"
class fruit :
	public stuff
{
	image * _img;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	virtual void trampled(float weight) {}
	//virtual void lifted(player* _player);
	virtual void used(player* _player);

	fruit() : stuff() {}
	~fruit() {}
};

