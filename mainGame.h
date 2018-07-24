#pragma once
#include "gameNode.h"
#include "loadingScene.h"
#include "nullScene.h"
#include "playingScene.h"
#include "dungeonScene.h"

//#include "enemyManager.h"
class mainGame : public gameNode
{
private:
	char _debug[128];

public:
	HRESULT init();			//�θ� virtual�̹Ƿ� �ڽ��� �ڵ����� virtual�� �ȴ�.
	void release();
	void update();
	void render();

	mainGame() {}
	~mainGame() {}
};

