#include "stdafx.h"
#include "townScene.h"


HRESULT townScene::init()
{
	//_testMap = IMAGEMANAGER->addImage("map", "Texture/player/1000x1000_map.bmp", 1000, 1000, true, RGB(255, 0, 255));
	_backgroundSky = IMAGEMANAGER->findImage("sky");
	_player = SAVEDATA->getPlayer();
	_player->init();

	//CAM->setRange(_testMap->getWidth(), _testMap->getHeight());

	_TownMap = IMAGEMANAGER->findImage("Town");
	_TownMapPixel = IMAGEMANAGER->findImage("TownPixel");

	_player->setMap(_TownMap);
	_player->setMapPixel(_TownMapPixel);

	CAM->setRange(_TownMap->getWidth(), _TownMap->getHeight());
	CAM->setPosition(_player->getX(), _player->getY());


	//SOUNDMANAGER->play("»ç¿îµå1");
	portal = RectMakeCenter(4000, 7500, 100, 100);

	

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

	g = new geddy;
	g->init();

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


	_stuffManager = new stuffManager;
	_stuffManager->init();


	_stuffManager->addStuff(0, _player->getX(), _player->getY());
	
	for (int i = 0; i < 20; ++i)
	{
		_stuffManager->addStuff(0, RND->getFromIntTo(200, _TownMap->getWidth() - 200),
									RND->getFromIntTo(200,_TownMap->getHeight() - 200));
	}

	_liftingActor = nullptr;

	return S_OK;
}

void townScene::release()
{
	RENDERMANAGER->clearBackground();

	_stuffManager->release();
	SAFE_DELETE(_stuffManager);

	SAFE_DELETE(g);
}

void townScene::update()
{
	_stuffManager->update();

	_player->update();

	if (KEYMANAGER->isStayKeyDown('1'))
	{
		RECT temp1;


		int i;
		for (i = 0; i < 20; ++i)
		{
			_liftingActor = _stuffManager->collide(_player);
			if (_liftingActor != NULL)
			{
				_player->changeState(FLY);
				_liftingActor->lifted(_player);
				break;
			}
		}
		
		if (i == 20 && IntersectRect(&temp1, &g->getHitbox(), &_player->getHitbox()))
		{
			g->lifted(_player);
			_player->setState(FLY);
		}
	}
	if (_liftingActor != nullptr && _liftingActor->getState() == HANG)
		_liftingActor->lifted(_player);
	else if(g->getState() == HANG)
		g->lifted(_player);

	if (KEYMANAGER->isOnceKeyDown('2'))
	{
		if (_liftingActor != nullptr && _liftingActor->getState() == HANG)
		{
			_liftingActor->throwed(10, getAngle(_liftingActor->getX() - CAM->getX(), _liftingActor->getY() - CAM->getY(), _ptMouse.x, _ptMouse.y));
		}
		else if (g->getState() == HANG)
		{
			g->throwed(10, getAngle(g->getX() - CAM->getX(), g->getY() - CAM->getY(), _ptMouse.x, _ptMouse.y));
		}
	}

	RECT temp;
	if (IntersectRect(&temp, &_player->getHitbox(), &portal))
	{
		SCENEMANAGER->loadScene("dungeonScene");
	}
	

	if (g->getState() == HANG && KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		tempCount = (tempCount + 1) % 5;
		if(tempCount == 0)
			g->attack();
	}



	g->update();

	if (g->getState() != 1)
	{
		CAM->videoShooting(_player->getX(), _player->getY());
	}

	for (int i = 0; i < 20; ++i)
	{
		_vRing[i]->update();
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
	RENDERMANAGER->addRender(g->getZ(), g);
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
