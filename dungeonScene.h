#pragma once
#include "gameNode.h"
#include "boss1.h"
class dungeonScene : public gameNode
{
	boss1* _boss1;
	image* _stage;
public:
	HRESULT init();
	void update();
	void render();
	void release();

	dungeonScene() {}
	~dungeonScene() {}
};

