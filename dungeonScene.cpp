#include "stdafx.h"
#include "dungeonScene.h"

HRESULT dungeonScene::init()
{
	IGM->addImage("������", "Texture/Maps/Boss1/dungeonMap_9235x1080.bmp", 9235, 1080);
	IGM->addImage("�������ȼ�", "Texture/Maps/Boss1/dungeonMapPixel_9235_1080.bmp", 9235, 1080);
	IGM->addImage("�������ͳ�", "Texture/Maps/Boss1/tunnel_658_420.bmp", 658, 420, true, MAGENTA);

	_player = SAVEDATA->getPlayer();
	_player->init();
	_player->setX(8960);
	_player->setY(584);

	_stage = IMAGEMANAGER->findImage("������");
	_stagePixel = IMAGEMANAGER->findImage("�������ȼ�");
	_stageTunnel = IMAGEMANAGER->findImage("�������ͳ�");

	_player->setMap(_stage);
	_player->setMapPixel(_stagePixel);

	_enemyManager = new enemyManager;
	initMonster();

	_enemyManager->init();
	_enemyManager->setPlayer(_player);
	_enemyManager->setMapPixel(_stagePixel);
	
	_player->setEnemyManager(_enemyManager);	// ���ʹ̸Ŵ����� �÷��̾��� ������ ��������ó�� �÷��̾ ���ʹ̸Ŵ����� ������ ��������.

	_geddy = new geddy;
	_geddy->init();
	_geddy->setMapPixel(_stagePixel);
	_geddy->setEnemyManager(_enemyManager);
	_geddy->setIsActive(false);

	_stuffManager = new stuffManager;
	_stuffManager->init();
	_shield->setMapPixel(_stagePixel);
	_stuffManager->addStuff(_shield);

	_stuffManager->addStuff(_geddy);

	_player->setStuffManager(_stuffManager);
	_player->setGeddy(_geddy);

	CAM->init();
	CAM->setRange(_stage->getWidth(), _stage->getHeight());
	CAM->setPosition(_player->getX(), _player->getY());

	SOUNDMANAGER->playBgm("����", _soundVolume);

	UIMANAGER->startingNewScene(_player->getX(), _player->getY());

	_enterBossDelay = 0;
	_isEnterBoss = false;

	return S_OK;
}

void dungeonScene::update()
{
	if (UIMANAGER->checkEndScene())
	{
		SCENEMANAGER->loadScene("endScene");
		return;
	}

	if (KEYMANAGER->isOnceKeyDown('1'))
	{
		if (!UIMANAGER->isChangingScene())
			UIMANAGER->startingSceneChange(_player->getX(), _player->getY());
		return;
	}
	if (KEYMANAGER->isOnceKeyDown('2'))
	{
		SCENEMANAGER->initScene();
	}

	_enemyManager->update();
	_stuffManager->update();
	_player->update();
	enterBossRoom();

	if(_isEnterBoss) CAM->videoShooting(_tortoise->getX(), _tortoise->getY(),15.0f);
	else CAM->videoShooting(_player->getX(), _player->getY());
}

void dungeonScene::render()
{
	_stage->render(getMemDC(), CAM->getSX(), CAM->getSY(), CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY); //ī�޶� ����ũ�� �̹����� CAM->getSX(), CAM->getSY()
	_enemyManager->render();
	_stuffManager->render();
	_player->render();
	_stageTunnel->render(getMemDC(), 1755 - CAM->getX() + CAM->getSX(), 400 - CAM->getY() + CAM->getSY());

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
	_tortoise->init(470, 850, LEFT); //init�Լ� �ȿ� setLink���� �����Ƿ� ���� 2�� _tortoise->setLink���� �ؾ���

	_shield->init(_tortoise->getX(), _tortoise->getY()); //_tortoise�� x,y�� �޾Ƽ� init�ؾ��ϹǷ� �� ������

	_enemyManager->addEnemy(_tortoise);
}

void dungeonScene::enterBossRoom()
{
	if (2300 < _player->getX() && _player->getX() < 2540 && !_tortoise->getIsStandby())
	{
		_isEnterBoss = true;
	}
	if (_isEnterBoss)
	{
		++_enterBossDelay;
	}
	if (_enterBossDelay >= 240) _isEnterBoss = false;
	else if (_enterBossDelay >= 20)
	{
		_tortoise->setIsStandby(true);
	}
	if (_player->getX() < 1750 && _tortoise->getIsStandby())
	{
		HPEN myPen, oldPen;
		HBRUSH myBrush, oldBrush;
		myBrush = CreateSolidBrush(RGB(0, 0, 0));
		oldBrush = (HBRUSH)SelectObject(_stagePixel->getMemDC(), myBrush);
		myPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(_stagePixel->getMemDC(), myPen);

		Rectangle(_stagePixel->getMemDC(), 1778,488,2394,710);
		SelectObject(_stagePixel->getMemDC(), oldBrush);
		SelectObject(_stagePixel->getMemDC(), oldPen);
		
		myBrush = CreateSolidBrush(RGB(255, 0, 255));
		oldBrush = (HBRUSH)SelectObject(_stageTunnel->getMemDC(), myBrush);
		myPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
		oldPen = (HPEN)SelectObject(_stageTunnel->getMemDC(), myPen);

		Rectangle(_stageTunnel->getMemDC(), 0, 0, 100, 420);
		SelectObject(_stageTunnel->getMemDC(), oldBrush);
		SelectObject(_stageTunnel->getMemDC(), oldPen);
		
		DeleteObject(myBrush);
		DeleteObject(oldBrush);
		DeleteObject(myPen);
		DeleteObject(oldPen);
	}
}

void dungeonScene::initMonster()
{
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
}
