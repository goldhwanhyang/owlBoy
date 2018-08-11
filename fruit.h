#pragma once
#include "liftableActor.h"
class fruit :
	public liftableActor
{
	image * _img;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	virtual void trampled(float weight) {}
	//virtual void lifted(player* _player);
	virtual liftableActor* use(actor* _player);

	fruit() : liftableActor() {}
	~fruit() {}
};

