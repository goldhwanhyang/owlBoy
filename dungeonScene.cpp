#include "stdafx.h"
#include "dungeonScene.h"

HRESULT dungeonScene::init()
{
	_player = SAVEDATA->getPlayer();
	_player->init();

	_stage = IMAGEMANAGER->findImage("던전맵");
	_stagePixel = IMAGEMANAGER->findImage("던전맵픽셀");
	_stageTunnel = new image;
	_stageTunnel->init("Texture/Maps/Boss1/tunnel_658_420.bmp", 658, 420, true, MAGENTA);
	//플레이어가 보스방으로 넘어간뒤에 터널이미지를 마젠타로 칠하면 런타임중 다시 init할때 터널이미지가 초기화 되지 않는다.
	//그걸 방지하기 위해서 비트맵 이미지를 새롭게 불러온다.

	RECT paintingRc = { 1778,488,2394,710 };
	painting(_stagePixel->getMemDC(), paintingRc, 255, 0, 255);

	_player->setMap(_stage);
	_player->setMapPixel(_stagePixel);

	_enemyManager = new enemyManager;
	initMonster();

	_enemyManager->init();
	_enemyManager->setPlayer(_player);
	_enemyManager->setMapPixel(_stagePixel);
	
	_player->setEnemyManager(_enemyManager);	// 에너미매니저가 플레이어의 정보를 가져간것처럼 플레이어도 에너미매니져의 정보를 가져간다.

	_geddy = new geddy;
	_geddy->init();
	_geddy->setMapPixel(_stagePixel);
	_geddy->setEnemyManager(_enemyManager);
	_geddy->setIsActive(false);

	_stuffManager = new stuffManager;
	_stuffManager->init();
	_shield->setMapPixel(_stagePixel);
	_stuffManager->addStuff(_shield);
	_stuffManager->setEnemyManager(_enemyManager);

	_stuffManager->addStuff(_geddy);

	/////////////////////////


	_stuffManager->addStuff(0, 8960, 584 + 60, _stagePixel);

	_stuffManager->addFruit(0, 8860, 584 + 50, _stagePixel);
	_stuffManager->addFruit(0, 8760, 584 + 50, _stagePixel);
	_stuffManager->addFruit(0, 8660, 584 + 50, _stagePixel);
	_stuffManager->addFruit(0, 8560, 584 + 50, _stagePixel);

	/////////////////////////

	_player->setStuffManager(_stuffManager);
	_player->setGeddy(_geddy);

	CAM->init();
	CAM->setRange(_stage->getWidth(), _stage->getHeight());
	CAM->setPosition(_player->getX(), _player->getY());

	SOUNDMANAGER->playBgm("던전", _soundVolume);
	SOUNDMANAGER->stop("던전");

	UIMANAGER->startingNewScene(_player->getX(), _player->getY());

	_enterBossDelay = 0;
	_isEnterBoss = false;

	_portal = RectMake(9000, 100, 100, 600);

	return S_OK;
}

void dungeonScene::update()
{
	if (UIMANAGER->checkEndScene())
	{
		if (!_tortoise->getIsActive())
			SCENEMANAGER->loadScene("endScene");
		else if(_player->getState() == DEAD)
		{
			SCENEMANAGER->initScene();
			_player->setHp(_player->getMaxHp());
			_player->changeState(IDLE);
		}
		else
		{
			// 던전 씬에서 마을 씬으로 이동할 때의 좌표
			_player->setX(2500);
			_player->setY(400);
			SCENEMANAGER->loadScene("townScene");
			_player->changeState(FLY);
		}
		return;
	}

	if (UIMANAGER->isChangingScene())
	{
		_player->frameSetting();
		CAM->videoShooting(_player->getX(), _player->getY());
		return;
	}

	if(!_tortoise->getIsActive())
	{
		if (!UIMANAGER->isChangingScene())
			UIMANAGER->startingSceneChange(_player->getX(), _player->getY());
		return;
	}

	RECT tempRc;
	if (IntersectRect(&tempRc, &_player->getHitbox(), &_portal))
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
	_stage->render(getMemDC(), CAM->getSX(), CAM->getSY(), CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY); //카메라 쉐이크할 이미지에 CAM->getSX(), CAM->getSY()
	_enemyManager->render();
	_stuffManager->render();
	_player->render();

	RENDERMANAGER->render(getMemDC());

	_stageTunnel->render(getMemDC(), 1755 - CAM->getX() + CAM->getSX(), 400 - CAM->getY() + CAM->getSY());
	
	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		_stagePixel->render(getMemDC(), CAM->getSX(), CAM->getSY(), CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY);
	}
}

void dungeonScene::release()
{
	SAFE_DELETE(_factory);
	_enemyManager->release();
	SAFE_DELETE(_enemyManager);
	_stuffManager->release();
	SAFE_DELETE(_stuffManager);
	_stageTunnel->release();
	SAFE_DELETE(_stageTunnel);
}

void dungeonScene::initTortoise()
{
	_tortoise = new tortoise;
	_shield = new tortoiseShield; //_tortoise->setShieldLink를 써야하므로 이 위치에서 동적할당

	_tortoise->setPlayerLink(_player);
	_tortoise->setMapPixel(_stagePixel);
	_tortoise->setShieldLink(_shield);
	_tortoise->init(470, 850, LEFT); //init함수 안에 setLink들이 있으므로 위의 2줄 _tortoise->setLink부터 해야함

	_shield->init(_tortoise->getX(), _tortoise->getY()); //_tortoise의 x,y를 받아서 init해야하므로 맨 마지막

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
		RECT paintingRc = { 1778,488,2394,710 };
		painting(_stagePixel->getMemDC(), paintingRc, 0, 0, 0);
		paintingRc = { 0,0,100,420 };
		painting(_stageTunnel->getMemDC(), paintingRc, 255, 0, 255);
	}
}

void dungeonScene::initMonster()
{
	_factory = new enemyFactory;

	for (int i = 0; i < 4; ++i)
	{
		enemy* _enemy = _factory->createEnemy(GAWK);
		_enemy->setPlayerLink(_player);	// 플레이어의 정보를 가져감(히트박스나 좌표 등등)
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

void dungeonScene::painting(HDC memDC, RECT location, int r, int g, int b)
{
	HPEN myPen, oldPen;
	HBRUSH myBrush, oldBrush;
	myBrush = CreateSolidBrush(RGB(r, g, b));
	oldBrush = (HBRUSH)SelectObject(memDC, myBrush);
	myPen = CreatePen(PS_SOLID, 1, RGB(r, g, b));
	oldPen = (HPEN)SelectObject(memDC, myPen);

	Rectangle(memDC, location);
	SelectObject(memDC, oldBrush);
	SelectObject(memDC, oldPen);

	DeleteObject(myBrush);
	DeleteObject(oldBrush);
	DeleteObject(myPen);
	DeleteObject(oldPen);
}
