#include "stdafx.h"
#include "dungeonScene.h"

HRESULT dungeonScene::init()
{
	_player = SAVEDATA->getPlayer();
	_player->init();

	_stage = IMAGEMANAGER->findImage("������");
	_stagePixel = IMAGEMANAGER->findImage("�������ȼ�");
	_stageTunnel = new image;
	_stageTunnel->init("Texture/Maps/Boss1/tunnel_658_420.bmp", 658, 420, true, MAGENTA);
	//�÷��̾ ���������� �Ѿ�ڿ� �ͳ��̹����� ����Ÿ�� ĥ�ϸ� ��Ÿ���� �ٽ� init�Ҷ� �ͳ��̹����� �ʱ�ȭ ���� �ʴ´�.
	//�װ� �����ϱ� ���ؼ� ��Ʈ�� �̹����� ���Ӱ� �ҷ��´�.

	RECT paintingRc = { 1778,488,2394,710 };
	painting(_stagePixel->getMemDC(), paintingRc, 255, 0, 255);

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

	SOUNDMANAGER->playBgm("����", _soundVolume);
	SOUNDMANAGER->stop("����");

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
			// ���� ������ ���� ������ �̵��� ���� ��ǥ
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
	_stage->render(getMemDC(), CAM->getSX(), CAM->getSY(), CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY); //ī�޶� ����ũ�� �̹����� CAM->getSX(), CAM->getSY()
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
