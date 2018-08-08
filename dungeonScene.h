#pragma once
#include "gameNode.h"
#include "player.h"
#include "enemyManager.h"
#include "stuffManager.h"

class dungeonScene : public gameNode
{
	image* _stage;
	image* _stagePixel;
	player* _player;
	enemyFactory* _factory;
	enemyManager* _enemyManager;
	stuffManager* _stuffManager;

	tortoise * _tortoise;
	tortoiseShield* _shield;
public:
	HRESULT init();
	void update();
	void render();
	void release();

	void setPlayer(player *p) { _player = p; }

	void initTortoise();
	void enterBossRoom();

	void initMonster();

	dungeonScene() {}
	~dungeonScene() {}
};

