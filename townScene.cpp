#include "stdafx.h"
#include "townScene.h"


HRESULT townScene::init()
{
	//_testMap = IMAGEMANAGER->addImage("map", "Texture/player/1000x1000_map.bmp", 1000, 1000, true, RGB(255, 0, 255));
	_backgroundSky = IMAGEMANAGER->findImage("sky");


	///////////////////////////////////////////

	
	_bush[0]._img = IMAGEMANAGER->findImage("풀_난");
	_bush[1]._img = IMAGEMANAGER->findImage("풀_난");
	_bush[2]._img = IMAGEMANAGER->findImage("풀_난");
	_bush[3]._img = IMAGEMANAGER->findImage("풀_난");
	_bush[4]._img = IMAGEMANAGER->findImage("풀_난");
	_bush[5]._img = IMAGEMANAGER->findImage("풀_난");
	_bush[6]._img = IMAGEMANAGER->findImage("풀_난");


	_bush[0]._pos = { 1571, 644 - _bush[0]._img->getFrameHeight() /2};
	_bush[1]._pos = { 2433, 2052 - _bush[1]._img->getFrameHeight() /2};
	_bush[2]._pos = { 773, 2946 - _bush[2]._img->getFrameHeight() /2};
	_bush[3]._pos = { 1809, 3049 - _bush[3]._img->getFrameHeight() /2};
	_bush[4]._pos = { 2560, 3815 - _bush[4]._img->getFrameHeight() /2};
	_bush[5]._pos = { 1419, 5223 - _bush[5]._img->getFrameHeight() /2};
	_bush[6]._pos = { 1678, 5123 - _bush[6]._img->getFrameHeight() /2};




	_backgroundObject[0]._img = IMAGEMANAGER->findImage("풀_바닥1");
	_backgroundObject[1]._img = IMAGEMANAGER->findImage("풀_바닥1");
	_backgroundObject[2]._img = IMAGEMANAGER->findImage("풀_바닥1");
	_backgroundObject[3]._img = IMAGEMANAGER->findImage("풀_바닥2");
	_backgroundObject[4]._img = IMAGEMANAGER->findImage("풀_바닥1");
	_backgroundObject[5]._img = IMAGEMANAGER->findImage("풀_바닥1");
	_backgroundObject[6]._img = IMAGEMANAGER->findImage("풀_바닥1");
	_backgroundObject[7]._img = IMAGEMANAGER->findImage("풀_바닥1");
	_backgroundObject[8]._img = IMAGEMANAGER->findImage("풀_바닥1");
	_backgroundObject[9]._img = IMAGEMANAGER->findImage("풀_바닥2");
	_backgroundObject[10]._img = IMAGEMANAGER->findImage("풀_바닥1");
	_backgroundObject[11]._img = IMAGEMANAGER->findImage("풀_바닥1");
	_backgroundObject[12]._img = IMAGEMANAGER->findImage("풀_바닥1");

	_backgroundObject[13]._img = IMAGEMANAGER->findImage("나무1");
	_backgroundObject[14]._img = IMAGEMANAGER->findImage("나무2");
	_backgroundObject[15]._img = IMAGEMANAGER->findImage("나무3");
	_backgroundObject[16]._img = IMAGEMANAGER->findImage("나무2");
	_backgroundObject[17]._img = IMAGEMANAGER->findImage("나무2");
	_backgroundObject[18]._img = IMAGEMANAGER->findImage("나무3");
	_backgroundObject[19]._img = IMAGEMANAGER->findImage("나무3");


	_backgroundObject[0]._pos = { 1571, 644};
	_backgroundObject[1]._pos = { 2433, 2052};
	_backgroundObject[2]._pos = { 773, 2946};
	_backgroundObject[3]._pos = { 1809, 3049};
	_backgroundObject[4]._pos = { 2560, 3815};
	_backgroundObject[5]._pos = { 1419, 5223};
	_backgroundObject[6]._pos = { 1678, 5123};
	_backgroundObject[7]._pos = { 2644, 6331};
	_backgroundObject[8]._pos = { 2910, 6085};
	_backgroundObject[9]._pos = { 3320, 6189};
	_backgroundObject[10]._pos = { 1531, 7147};
	_backgroundObject[11]._pos = { 3191, 7287};
	_backgroundObject[12]._pos = { 1173, 2946};

	_backgroundObject[13]._pos = { 1809, 3049 - _backgroundObject[13]._img->getFrameHeight() + 30 };
	_backgroundObject[14]._pos = { 2560, 3815 - _backgroundObject[14]._img->getFrameHeight() + 30 };
	_backgroundObject[15]._pos = { 1419, 5223 - _backgroundObject[15]._img->getFrameHeight() + 30 };
	_backgroundObject[16]._pos = { 1678, 5123 - _backgroundObject[13]._img->getFrameHeight() + 30 };
	_backgroundObject[17]._pos = { 2910, 6085 - _backgroundObject[14]._img->getFrameHeight() + 30 };
	_backgroundObject[18]._pos = { 3320, 6189 - _backgroundObject[15]._img->getFrameHeight() + 30 };
	_backgroundObject[19]._pos = { 2644, 6331 - _backgroundObject[13]._img->getFrameHeight() + 30 };

	_bushFrameX = 0;
	_bushFrameY = 0;
	_bushFrameX2 = _bushFrameY2 = 0;
	_bushCount = 0;




	///////////////////////////////////////////
	
	_bottomCloud = IMAGEMANAGER->findImage("마을_바닥");
	_topCloud = IMAGEMANAGER->findImage("마을_천장");
	_player = SAVEDATA->getPlayer();
	_player->init();
	_player->setX(1963.f);
	_player->setY(7121.f);
	
	//CAM->setRange(_testMap->getWidth(), _testMap->getHeight());

	_TownMap = IMAGEMANAGER->findImage("Town");
	_TownMapPixel = IMAGEMANAGER->findImage("TownPixel");

	_player->setMap(_TownMap);
	_player->setMapPixel(_TownMapPixel);

	CAM->setRange(_TownMap->getWidth(), _TownMap->getHeight());
	CAM->setPosition(_player->getX(), _player->getY());


	//SOUNDMANAGER->play("사운드1");
	portal = RectMakeCenter(_TownMap->getWidth() / 2, 100, 200, 200);

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

	//////////////////////////
	ring *temp_ring[20];
	for (int i = 0; i < 20; ++i)
	{
		temp_ring[i] = new ring;
		_vRing.push_back(temp_ring[i]);
	}


	temp_ring[0]->init(1963.f - 1000, 7121.f, 1);
	temp_ring[1]->init(1963.f - 1000, 6921.f, 1);
	temp_ring[2]->init(1963.f - 1000, 6721.f, 1);
	temp_ring[3]->init(1963.f - 1000, 6521.f, 1);

	temp_ring[4]->init(1963.f + 300, 7121.f, 1);
	temp_ring[5]->init(1963.f + 300, 6921.f, 1);
	temp_ring[6]->init(1963.f + 300, 6721.f, 1);
	temp_ring[7]->init(1963.f + 300, 6521.f, 1);


	temp_ring[8]->init(_TownMap->getWidth() / 2, 400, 1);
	temp_ring[9]->init(_TownMap->getWidth() / 2, 600, 1);
	temp_ring[10]->init(_TownMap->getWidth() / 2, 800, 1);
	temp_ring[11]->init(_TownMap->getWidth() / 2, 1000, 1);

	temp_ring[12]->init(3100, 5000, 1);
	temp_ring[13]->init(3200, 5750, 0);
	temp_ring[14]->init(2400, 6000, 2);
	temp_ring[15]->init(2400, 6300, 1);
	temp_ring[16]->init(2400, 6800, 3);
	temp_ring[17]->init(3300, 6800, 0);
	temp_ring[18]->init(3900, 6700, 2);
	temp_ring[19]->init(3900, 5450, 1);

	for (int i = 0; i < 20; ++i)
	{
		_vRing.push_back(temp_ring[i]);
	}

	////////////////////////////

	_geddy = new geddy;
	_geddy->init();
	_geddy->setMapPixel(_TownMapPixel);
	_geddy->setIsActive(false);

	_stuffManager = new stuffManager;
	_stuffManager->init();
	_stuffManager->addStuff(_geddy);

	
	_stuffManager->addStuff(0, 1571 + RND->getFromIntTo(100, 300), 644 + 40);
	_stuffManager->addStuff(0, 2433 + RND->getFromIntTo(100, 300), 2052 + 40);
	_stuffManager->addStuff(0, 773 + RND->getFromIntTo(100, 300), 2946 + 40);
	_stuffManager->addStuff(0, 1809 + RND->getFromIntTo(100, 300), 3049 + 40);
	_stuffManager->addStuff(0, 2560 + RND->getFromIntTo(100, 300), 3815 + 40);
	_stuffManager->addStuff(0, 1419 + RND->getFromIntTo(100, 300), 5223 + 40);
	_stuffManager->addStuff(0, 1678 + RND->getFromIntTo(100, 300), 5123 + 40);
	_stuffManager->addStuff(0, 2644 + RND->getFromIntTo(100, 300), 6331 + 40);
	_stuffManager->addStuff(0, 2910 + RND->getFromIntTo(100, 300), 6085 + 40);
	_stuffManager->addStuff(0, 3320 + RND->getFromIntTo(100, 300), 6189 + 40);
	_stuffManager->addStuff(0, 1531 + RND->getFromIntTo(100, 300), 7147 + 40);
	_stuffManager->addStuff(0, 3191 + RND->getFromIntTo(100, 300), 7287 + 40);


	_stuffManager->addFruit(0, 1571 + RND->getFromIntTo(400, 600), 644 +  30);
	_stuffManager->addFruit(0, 2433 + RND->getFromIntTo(400, 600), 2052 + 30);
	_stuffManager->addFruit(0, 773 + RND->getFromIntTo(400,  600), 2946 + 30);
	//_stuffManager->addFruit(0, 1809 + RND->getFromIntTo(500, 700), 3049 + 30);
	_stuffManager->addFruit(0, 2560 + RND->getFromIntTo(400, 600), 3815 + 30);
	_stuffManager->addFruit(0, 1419 + RND->getFromIntTo(400, 600), 5223 + 30);
	//_stuffManager->addFruit(0, 1678 + RND->getFromIntTo(500, 700), 5123 + 30);
	_stuffManager->addFruit(0, 2644 + RND->getFromIntTo(400, 600), 6331 + 30);
	//_stuffManager->addFruit(0, 2910 + RND->getFromIntTo(500, 700), 6085 + 30);
	//_stuffManager->addFruit(0, 3320 + RND->getFromIntTo(500,  700), 6189 + 30);
	_stuffManager->addFruit(0, 1531 + RND->getFromIntTo(400, 600), 7147 + 30);
	_stuffManager->addFruit(0, 3191 + RND->getFromIntTo(400, 600), 7287 + 30);


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

	CAM->videoShooting(_player->getX(), _player->getY());
	
	for (int i = 0; i < 20; ++i)
	{
		if (!_vRing[i]->getIsActive()) continue;

		_vRing[i]->update();
		_vRing[i]->collide(_player);
	}

	_bushCount = (_bushCount + 1) % 20;
	if (_bushCount == 0)
	{
		_bushFrameX = (_bushFrameX + 1) % 3;
		_bushFrameY = (_bushFrameY + 1) % 3;
		_bushFrameX2 = (_bushFrameX2 + 1) % 10;
		_bushFrameY2 = (_bushFrameY2 + 1) % 10;
	}
}

void townScene::render()
{
	_backgroundSky->render(getMemDC(), 0, 0);
	RENDERMANAGER->backgroundRender(getMemDC());

	for (int i = 13; i < MAX_FRAME_3; ++i)
	{
		_backgroundObject[i]._img->frameRender(getMemDC(), _backgroundObject[i]._pos.x - CAM->getX() + CAM->getSX(), _backgroundObject[i]._pos.y - CAM->getY() + CAM->getSX(), _bushFrameX, _bushFrameY);
	}

	//_testMap->render(getMemDC());
	_TownMap->render(getMemDC(), CAM->getSX(), CAM->getSY(), CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY);

	for (int i = 0; i < 12; ++i)
	{
		_backgroundObject[i]._img->frameRender(getMemDC(), _backgroundObject[i]._pos.x - CAM->getX() + CAM->getSX(), _backgroundObject[i]._pos.y - CAM->getY() + CAM->getSX(), _bushFrameX, _bushFrameY);
	}


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
		if(_vRing[i]->getIsActive())
			_vRing[i]->renderBack();
	}
	RENDERMANAGER->render(getMemDC());
	for (int i = 0; i < 20; ++i)
	{
		if (_vRing[i]->getIsActive())
			_vRing[i]->renderFront();
	}

	for (int i = 0; i < MAX_FRAME_10; ++i)
	{
		_bush[i]._img->frameRender(getMemDC(), _bush[i]._pos.x - CAM->getX() + CAM->getSX(), _bush[i]._pos.y - CAM->getY() + CAM->getSX(), _bushFrameX2, _bushFrameY2);
	}

	_topCloud->render(getMemDC(), CAM->getSX() - CAM->getX(), CAM->getSY() - CAM->getY());
	_bottomCloud->render(getMemDC(), CAM->getSX() - CAM->getX(), _TownMap->getHeight() -_bottomCloud->getHeight() - CAM->getY());
}
