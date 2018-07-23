#pragma once
#include "gameNode.h"
#include "loadingScene.h"
#include "nullScene.h"
//#include "enemyManager.h"
class mainGame : public gameNode
{
private:
	char _debug[128];

public:
	HRESULT init();			//부모가 virtual이므로 자식은 자동으로 virtual이 된다.
	void release();
	void update();
	void render();

	mainGame() {}
	~mainGame() {}
};

