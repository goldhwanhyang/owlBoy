#pragma once
#include "gameNode.h"
class nullScene : public gameNode
{
private:
	//image * _bgImage;
	//image* _face;				//추가 이미지
	
	RECT _rc;					//렉트

	//게임씬
	/*aaa* _startScene;
	bbb* _gameScene;
	gameNode* _currentScene;*/

	//에너미 매니저
	/*
	enemyManager* _enemyManager;
	*/
public:
	HRESULT init();
	void release();
	void update();
	void render();

	void collision();

	nullScene() {}
	~nullScene() {}
};

