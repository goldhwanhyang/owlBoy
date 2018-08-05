#pragma once
#include "gameNode.h"
class startScene :
	public gameNode
{
	image *_bgImage, *_bgCloud, *_stone1, *_stone2;
	
	int _count, _index;

	float _angle;
public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	startScene() {}
	~startScene() {}
};

