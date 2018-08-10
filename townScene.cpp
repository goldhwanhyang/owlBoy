#include "stdafx.h"
#include "townScene.h"


HRESULT townScene::init()
{
	//_testMap = IMAGEMANAGER->addImage("map", "Texture/player/1000x1000_map.bmp", 1000, 1000, true, RGB(255, 0, 255));
	_backgroundSky = IMAGEMANAGER->findImage("sky");
	_player = SAVEDATA->getPlayer();
	_player->init();
	_player->setX(440.f);
	_player->setY(810.f);
	
	//CAM->setRange(_testMap->getWidth(), _testMap->getHeight());

	_TownMap = IMAGEMANAGER->findImage("Town");
	_TownMapPixel = IMAGEMANAGER->findImage("TownPixel");

	_player->setMap(_TownMap);
	_player->setMapPixel(_TownMapPixel);

	CAM->setRange(_TownMap->getWidth(), _TownMap->getHeight());
	CAM->setPosition(_player->getX(), _player->getY());


	//SOUNDMANAGER->play("사운드1");
	portal = RectMakeCenter(WINSIZEX/2, 50, 100, 100);

	cloud *temp_cloud;
	for (int i = 0; i < 100; ++i)
	{
		temp_cloud = new cloud;
		temp_cloud->init();
		RENDERMANAGER->addBackground(temp_cloud->getZ(), temp_cloud);
	}
	stone *temp_stone;
	for (int i = 0; i < 20; ++i)
	{
		temp_stone = new stone;
		temp_stone->init();
		RENDERMANAGER->addBackground(temp_stone->getZ(), temp_stone);
	}
	
	SOUNDMANAGER->playBgm("마을",_soundVolume);

/*
	tR.init(_player->getX() + 500, _player->getY() - 300, 1);
	*/

	ring *temp_ring;
	for (int i = 0; i < 20; ++i)
	{
		temp_ring = new ring;
		temp_ring->init(RND->getFromIntTo(200, _TownMap->getWidth() - 200),
						RND->getFromIntTo(200, _TownMap->getHeight() - 200),
						RND->getInt(4));
		_vRing.push_back(temp_ring);
	}


	_geddy = new geddy;
	_geddy->init();
	_geddy->setMapPixel(_TownMapPixel);

	_stuffManager = new stuffManager;
	_stuffManager->init();
	_stuffManager->addStuff(_geddy);

	for (int i = 0; i < 20; ++i)
	{
		_stuffManager->addStuff(0, RND->getFromIntTo(200, _TownMap->getWidth() - 200),
									RND->getFromIntTo(200,_TownMap->getHeight() - 200));
	}
	for (int i = 0; i < 20; ++i)
	{
		_stuffManager->addFruit(0, RND->getFromIntTo(200, _TownMap->getWidth() - 200),
			RND->getFromIntTo(200, _TownMap->getHeight() - 200));
	}


	_player->setStuffManager(_stuffManager);
	_player->setEnemyManager(NULL);

	_player->setGeddy(_geddy);

	UIMANAGER->startingNewScene(_player->getX(), _player->getY());
	endScene = false;
	return S_OK;
}

void townScene::release()
{
	RENDERMANAGER->clearBackground();

	_stuffManager->release();
	SAFE_DELETE(_stuffManager);
}

void townScene::update()
{
	RECT temp;

	if (UIMANAGER->checkEndScene())
	{
		SCENEMANAGER->loadScene("dungeonScene");
		return;
	}

	if (IntersectRect(&temp, &_player->getHitbox(), &portal))
	{
		if (!UIMANAGER->isChangingScene())
			UIMANAGER->startingSceneChange(_player->getX(), _player->getY());
		return;
	}

	_stuffManager->update();

	_player->update();
	if (KEYMANAGER->isOnceKeyDown('1'))
	{
		UIMANAGER->flickering(RGB(255, 0, 0), 10, 1);
	}

	

	CAM->videoShooting(_player->getX(), _player->getY());
	
	for (int i = 0; i < 20; ++i)
	{
		_vRing[i]->update();
		_vRing[i]->collide(_player);
	}
}

void townScene::render()
{
	_backgroundSky->render(getMemDC(), 0, 0);
	RENDERMANAGER->backgroundRender(getMemDC());

	//_testMap->render(getMemDC());
	_TownMap->render(getMemDC(), CAM->getSX(), CAM->getSY(), CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY);
	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		_TownMapPixel->render(getMemDC(), CAM->getSX(), CAM->getSY(), CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY);
		Rectangle(getMemDC(), portal.left - CAM->getX(), portal.top - CAM->getY(), portal.right - CAM->getX(), portal.bottom - CAM->getY());
	}
	//if(_geddy->getState() != geddyEnum::HANG)
	//	RENDERMANAGER->addRender(_geddy->getZ(), _geddy);
	RENDERMANAGER->addRender(_player->getZ(), _player);
	_stuffManager->render();

	for (int i = 0; i < 20; ++i)
	{
		_vRing[i]->renderBack();
	}
	RENDERMANAGER->render(getMemDC());
	for (int i = 0; i < 20; ++i)
	{
		_vRing[i]->renderFront();
	}

}
