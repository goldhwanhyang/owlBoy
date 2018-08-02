#pragma once
#include "gameNode.h"
#include "tortoise.h"
#include "gawk.h"
#include "player.h"

class dungeonScene : public gameNode
{
	tortoise* _tortoise;
	tortoiseShield* _shield;
	image* _stage;
	image* _stagePixel;
	vector<enemy*> _vEnemy;
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

