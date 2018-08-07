#include "stdafx.h"
#include "dungeonScene.h"

HRESULT dungeonScene::init()
{
	IGM->addImage("´øÀü¸Ê", "Texture/Maps/Boss1/dungeonMap_9235x1080.bmp", 9235, 1080);
	IGM->addImage("´øÀü¸ÊÇÈ¼¿", "Texture/Maps/Boss1/dungeonMapPixel_9235_1080.bmp", 9235, 1080);

	_player = SAVEDATA->getPlayer();
	_player->init();
	_player->setX(8960);
	_player->setY(584);

	//_player->setX(420);
	//_player->setY(937);

	//_stage = IGM->findImage("º¸½º¹æ1");
	//_stagePixel = IMAGEMANAGER->findImage("º¸½º¹æ1ÇÈ¼¿");

	_stage = IGM->findImage("´øÀü¸Ê");
	_stagePixel = IMAGEMANAGER->findImage("´øÀü¸ÊÇÈ¼¿");

	_player->setMap(_stage);
	_player->setMapPixel(_stagePixel);

	_enemyManager = new enemyManager;
	_enemyManager->setPlayer(_player);
	_enemyManager->setMapPixel(_stagePixel);
	_enemyManager->init();

	CAM->init();
	CAM->setRange(_stage->getWidth(), _stage->getHeight());
	CAM->setPosition(_player->getX(), _player->getY());

	return S_OK;
}

void dungeonScene::update()
{
	_enemyManager->update();
	_player->update();
	enterBossRoom();

	CAM->videoShooting(_player->getX(), _player->getY());
}

void dungeonScene::render()
{
	_stage->render(getMemDC(), CAM->getSX(), CAM->getSY(), CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY); //Ä«¸Þ¶ó ½¦ÀÌÅ©ÇÒ ÀÌ¹ÌÁö¿¡ CAM->getSX(), CAM->getSY()
	_enemyManager->render();
	_player->render();

	RENDERMANAGER->render(getMemDC());
}

void dungeonScene::release()
{
	_enemyManager->release();
	SAFE_DELETE(_enemyManager);
}

void dungeonScene::enterBossRoom()
{
	if (2000 < _player->getX() && _player->getX() < 2540)
	{
		_player->setX(420);
		_player->setY(937);
	}
}
