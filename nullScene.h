#pragma once
#include "gameNode.h"
class nullScene : public gameNode
{
private:
	//image * _bgImage;
	//image* _face;				//�߰� �̹���
	
	RECT _rc;					//��Ʈ

	//���Ӿ�
	/*aaa* _startScene;
	bbb* _gameScene;
	gameNode* _currentScene;*/

	//���ʹ� �Ŵ���
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

