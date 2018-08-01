#pragma once
#include "gameNode.h"
#include "player.h"
#include "effect.h"
#include "ironWeight.h"
#include "cloud.h"
#include "stone.h"
#include "geddy.h"

class townScene : public gameNode
{
private:
	//image* _testMap;
	image * _backgroundSky;
	image * _TownMap;
	image* _TownMapPixel;
	player* _player;
	RECT portal;

	ironWeight temp;
	geddy * g;

public:

	HRESULT init();
	void release();
	void update();
	void render();

	townScene() {}
	~townScene() {}
};

