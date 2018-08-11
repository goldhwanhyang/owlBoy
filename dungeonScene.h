#pragma once
#include "gameNode.h"
#include "geddy.h"
#include "player.h"
#include "enemyManager.h"
#include "stuffManager.h"

class dungeonScene : public gameNode
{
	image* _stage;
	image* _stagePixel;
	image* _stageTunnel;
	player* _player;
	enemyFactory* _factory;
	enemyManager* _enemyManager;
	stuffManager* _stuffManager;

	geddy *_geddy;

	tortoise * _tortoise;
	tortoiseShield* _shield;

	RECT _portal;

	int _enterBossDelay;
	bool _isEnterBoss;
public:
	HRESULT init();
	void update();
	void render();
	void release();

	void setPlayer(player *p) { _player = p; }

	void initTortoise();
	void enterBossRoom();

	void initMonster();
	void painting(HDC memDC, RECT location, int r, int g, int b);

	dungeonScene() {}
	~dungeonScene() {}
};

