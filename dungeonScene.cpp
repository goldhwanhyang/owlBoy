#include "stdafx.h"
#include "dungeonScene.h"

HRESULT dungeonScene::init()
{
	IGM->addImage("������", "Texture/Maps/Boss1/dungeonMap_9235x1080.bmp", 9235, 1080);
	IGM->addImage("�������ȼ�", "Texture/Maps/Boss1/dungeonMapPixel_9235_1080.bmp", 9235, 1080);

	_player = SAVEDATA->getPlayer();
	_player->init();
	_player->setX(8960);
	_player->setY(584);

	//_player->setX(420);
	//_player->setY(937);

	//_stage = IGM->findImage("������1");
	//_stagePixel = IMAGEMANAGER->findImage("������1�ȼ�");

	_stage = IGM->findImage("������");
	_stagePixel = IMAGEMANAGER->findImage("�������ȼ�");
	_enemyManager = new enemyManager;

	_player->setMap(_stage);
	_player->setMapPixel(_stagePixel);

	_enemyManager->setMapPixel(_stagePixel);
	_enemyManager->setPlayer(_player);
	_enemyManager->init();
	
	_player->setEnemyManager(_enemyManager);	// ���ʹ̸Ŵ����� �÷��̾��� ������ ��������ó�� �÷��̾ ���ʹ̸Ŵ����� ������ ��������.

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

void dungeonScene::enterBossRoom()
{
	if (2000 < _player->getX() && _player->getX() < 2540)
	{
		_player->setX(420);
		_player->setY(937);
	}
}
