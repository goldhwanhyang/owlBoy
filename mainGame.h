#pragma once
#include "gameNode.h"
#include "loadingScene.h"
#include "nullScene.h"
#include "townScene.h"
#include "dungeonScene.h"
#include "startScene.h"
#include "endScene.h"
#include "player.h"

//#include "enemyManager.h"
class mainGame : public gameNode
{
private:
	char _debug[128];
	player *_player;
public:
	HRESULT init();			//�θ� virtual�̹Ƿ� �ڽ��� �ڵ����� virtual�� �ȴ�.
	void release();
	void update();
	void render();

	mainGame() {}
	~mainGame() {}
};

