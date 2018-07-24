#pragma once
#include "gameNode.h"
#include "player.h"
class playingScene : public gameNode
{
private:
	//image* _testMap;
	player* _player;
public:

	HRESULT init();
	void release();
	void update();
	void render();

	playingScene() {}
	~playingScene() {}
};

