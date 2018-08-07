#pragma once
#include "gameNode.h"
#include "player.h"
#include "enemyManager.h"

class dungeonScene : public gameNode
{
	image* _stage;
	image* _stagePixel;
	player* _player;
	enemyManager* _enemyManager;
public:
	HRESULT init();
	void update();
	void render();
	void release();

	void setPlayer(player *p) { _player = p; }

	void enterBossRoom();

	dungeonScene() {}
	~dungeonScene() {}
};

