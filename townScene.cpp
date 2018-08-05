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


	//SOUNDMANAGER->play("사운드1");
	portal = RectMakeCenter(4000, 7500, 100, 100);

	temp.init();
	tR.init(_player->getX() + 500, _player->getY() - 300, 1);
	temp.setX(_player->getX());
	temp.setY(_player->getY());

	cloud *temp;
	for (int i = 0; i < 100; ++i)
	{
		temp = new cloud;
		temp->init();
		RENDERMANAGER->addBackground(temp->getZ(), temp);
	}
	stone *temp1;
	for (int i = 0; i < 20; ++i)
	{
		temp1 = new stone;
		temp1->init();
		RENDERMANAGER->addBackground(temp1->getZ(), temp1);
	}

	g = new geddy;
	g->init();
	return S_OK;
}

void townScene::release()
{
	RENDERMANAGER->clearBackground();

	SAFE_DELETE(g);
}

void townScene::update()
{
	_player->update();

	if (KEYMANAGER->isStayKeyDown('1'))
	{
		RECT temp1;
		if (IntersectRect(&temp1, &temp.getHitbox(), &_player->getHitbox()))
		{
			temp.lifted(_player);
			_player->setState(FLY);
		}
		else if (IntersectRect(&temp1, &g->getHitbox(), &_player->getHitbox()))
		{
			g->lifted(_player);
			_player->setState(FLY);
		}
	}
	if (temp.getState() == HANG)
		temp.lifted(_player);
	else if(g->getState() == HANG)
		g->lifted(_player);

	if (KEYMANAGER->isOnceKeyDown('2'))
	{
		if (temp.getState() == HANG)
		{
			temp.throwed(10, getAngle(temp.getX() - CAM->getX(), temp.getY() - CAM->getY(), _ptMouse.x, _ptMouse.y));
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
	

	/*이펙트 매니져 사용시*/
	if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
	{
		EFFECTMANAGER->play("폭발", _ptMouse.x, _ptMouse.y);
	}

	if (g->getState() == HANG && KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		tempCount = (tempCount + 1) % 5;
		if(tempCount == 0)
			g->attack();
	}


	this->temp.update();
	g->update();

	if (g->getState() != 1)
	{
		CAM->videoShooting(_player->getX(), _player->getY());
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
	RENDERMANAGER->addRender(temp.getZ(), &temp);
	RENDERMANAGER->addRender(g->getZ(), g);
	RENDERMANAGER->addRender(_player->getZ(), _player);

	tR.renderBack();
	RENDERMANAGER->render(getMemDC());
	tR.renderFront();
}
