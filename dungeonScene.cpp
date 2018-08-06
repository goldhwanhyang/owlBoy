#include "stdafx.h"
#include "dungeonScene.h"

HRESULT dungeonScene::init()
{
	IGM->addImage("������", "Texture/Maps/Boss1/dungeonMap.bmp", 6840, 1080);
	IGM->addImage("�������ȼ�", "Texture/Maps/Boss1/dungeonMapPixel.bmp", 6840, 1080);

	_player = SAVEDATA->getPlayer();
	_player->init();
	_player->setX(1000);
	_player->setY(500);

	_stage = IGM->findImage("������1");
	_stagePixel = IMAGEMANAGER->findImage("������1�ȼ�");

	//_stage = IGM->findImage("������");
	//_stagePixel = IMAGEMANAGER->findImage("�������ȼ�");

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

	CAM->videoShooting(_player->getX(), _player->getY());
}

void dungeonScene::render()
{
	_stage->render(getMemDC(), CAM->getSX(), CAM->getSY(), CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY); //ī�޶� ����ũ�� �̹����� CAM->getSX(), CAM->getSY()
	_enemyManager->render();
	_player->render();

	RENDERMANAGER->render(getMemDC());
}

void dungeonScene::release()
{
	_enemyManager->release();
	SAFE_DELETE(_enemyManager);
}
