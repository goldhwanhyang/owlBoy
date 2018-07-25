#pragma once
#include "gameNode.h"
#include "boss1.h"
#include "gawk.h"
#include "player.h"

class dungeonScene : public gameNode
{
	boss1* _boss1;
	image* _stage;
	image* _stagePixel;
	enemy* _gawk1;
	player* _player;
public:
	HRESULT init();
	void update();
	void render();
	void release();

	void setPlayer(player *p) { _player = p; }

	dungeonScene() {}
	~dungeonScene() {}
};

