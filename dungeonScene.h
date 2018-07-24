#pragma once
#include "gameNode.h"
class dungeonScene : public gameNode
{
	image* _stage;
public:
	HRESULT init();
	void update();
	void render();
	void release();

	dungeonScene() {}
	~dungeonScene() {}
};

