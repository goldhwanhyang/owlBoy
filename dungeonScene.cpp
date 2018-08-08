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

	_stage = IGM->findImage("������");
	_stagePixel = IMAGEMANAGER->findImage("�������ȼ�");

	_player->setMap(_stage);
	_player->setMapPixel(_stagePixel);

	_enemyManager = new enemyManager;
	_factory = new enemyFactory;

	for (int i = 0; i < 4; ++i)
	{
		enemy* _enemy = _factory->createEnemy(GAWK);
		_enemy->setPlayerLink(_player);	// �÷��̾��� ������ ������(��Ʈ�ڽ��� ��ǥ ���)
		_enemy->setMapPixel(_stagePixel);
		_enemyManager->addEnemy(_enemy);
	}
	for (int i = 0; i < 4; ++i)
	{
		enemy* _enemy = _factory->createEnemy(TORQUE);
		_enemy->setPlayerLink(_player);
		_enemy->setMapPixel(_stagePixel);
		_enemyManager->addEnemy(_enemy);
	}

	_enemyManager->setPostion(0, 5600, 192, RIGHT);
	_enemyManager->setPostion(1, 6100, 192, RIGHT);
	_enemyManager->setPostion(2, 7480, 192, RIGHT);
	_enemyManager->setPostion(3, 7590, 192, RIGHT);
	_enemyManager->setPostion(4, 3045, 943, RIGHT);
	_enemyManager->setPostion(5, 3640, 943, RIGHT);
	_enemyManager->setPostion(6, 4675, 883, LEFT);
	_enemyManager->setPostion(7, 6030, 912, RIGHT);

	_enemyManager->setPostion(3, 8428, 192, RIGHT);
	_enemyManager->setPostion(4, 8428, 578, RIGHT);

	initTortoise();

	_enemyManager->init();
	_enemyManager->setPlayer(_player);
	_enemyManager->setMapPixel(_stagePixel);
	
	_player->setEnemyManager(_enemyManager);	// ���ʹ̸Ŵ����� �÷��̾��� ������ ��������ó�� �÷��̾ ���ʹ̸Ŵ����� ������ ��������.

	_stuffManager = new stuffManager;
	_stuffManager->init();
	_stuffManager->addStuff(_shield);

	_player->setStuffManager(_stuffManager);

	CAM->init();
	CAM->setRange(_stage->getWidth(), _stage->getHeight());
	CAM->setPosition(_player->getX(), _player->getY());

	return S_OK;
}

void dungeonScene::update()
{
	_enemyManager->update();
	_stuffManager->update();
	_player->update();
	enterBossRoom();

	CAM->videoShooting(_player->getX(), _player->getY());
}

void dungeonScene::render()
{
	_stage->render(getMemDC(), CAM->getSX(), CAM->getSY(), CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY); //ī�޶� ����ũ�� �̹����� CAM->getSX(), CAM->getSY()
	_enemyManager->render();
	_stuffManager->render();
	_player->render();

	RENDERMANAGER->render(getMemDC());
}

void dungeonScene::release()
{
	SAFE_DELETE(_factory);
	_enemyManager->release();
	SAFE_DELETE(_enemyManager);
	_stuffManager->release();
	SAFE_DELETE(_stuffManager);
}

void dungeonScene::initTortoise()
{
	_tortoise = new tortoise;
	_shield = new tortoiseShield; //_tortoise->setShieldLink�� ����ϹǷ� �� ��ġ���� �����Ҵ�

	_tortoise->setPlayerLink(_player);
	_tortoise->setMapPixel(_stagePixel);
	_tortoise->setShieldLink(_shield);
	_tortoise->init(1400, 850, LEFT); //init�Լ� �ȿ� setLink���� �����Ƿ� ���� 2�� _tortoise->setLink���� �ؾ���

	_shield->init(_tortoise->getX(), _tortoise->getY()); //_tortoise�� x,y�� �޾Ƽ� init�ؾ��ϹǷ� �� ������

	_enemyManager->addEnemy(_tortoise);
}

void dungeonScene::enterBossRoom()
{
	if (2000 < _player->getX() && _player->getX() < 2540)
	{
		_player->setX(420);
		_player->setY(937);
	}
}

void dungeonScene::initMonster()
{
}
