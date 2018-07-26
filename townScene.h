#pragma once
#include "gameNode.h"
#include "player.h"
#include "effect.h"
class townScene : public gameNode
{
private:
	//image* _testMap;
	image * _TownMap;
	image* _TownMapPixel;
	player* _player;
	RECT portal;

public:

	HRESULT init();
	void release();
	void update();
	void render();

	townScene() {}
	~townScene() {}
};

