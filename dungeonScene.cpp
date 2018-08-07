#include "stdafx.h"
#include "dungeonScene.h"

HRESULT dungeonScene::init()
{
	IGM->addImage("던전맵", "Texture/Maps/Boss1/dungeonMap_9235x1080.bmp", 9235, 1080);
	IGM->addImage("던전맵픽셀", "Texture/Maps/Boss1/dungeonMapPixel_9235_1080.bmp", 9235, 1080);

	_player = SAVEDATA->getPlayer();
	_player->init();
	_player->setX(8960);
	_player->setY(584);

	//_player->setX(420);
	//_player->setY(937);

	//_stage = IGM->findImage("보스방1");
	//_stagePixel = IMAGEMANAGER->findImage("보스방1픽셀");

	_stage = IGM->findImage("던전맵");
	_stagePixel = IMAGEMANAGER->findImage("던전맵픽셀");
	_enemyManager = new enemyManager;

	_player->setMap(_stage);
	_player->setMapPixel(_stagePixel);

	_enemyManager->setMapPixel(_stagePixel);
	_enemyManager->setPlayer(_player);
	_enemyManager->init();
	
	_player->setEnemyManager(_enemyManager);	// 에너미매니저가 플레이어의 정보를 가져간것처럼 플레이어도 에너미매니져의 정보를 가져간다.

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
	_stage->render(getMemDC(), CAM->getSX(), CAM->getSY(), CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY); //카메라 쉐이크할 이미지에 CAM->getSX(), CAM->getSY()
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
