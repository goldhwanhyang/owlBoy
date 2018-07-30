#include "stdafx.h"
#include "townScene.h"


HRESULT townScene::init()
{
	//_testMap = IMAGEMANAGER->addImage("map", "Texture/player/1000x1000_map.bmp", 1000, 1000, true, RGB(255, 0, 255));
	_player = SAVEDATA->getPlayer();
	_player->init();

	//CAM->setRange(_testMap->getWidth(), _testMap->getHeight());

	_TownMap = IMAGEMANAGER->findImage("Town");
	_TownMapPixel = IMAGEMANAGER->findImage("TownPixel");

	_player->setMap(_TownMap);
	_player->setMapPixel(_TownMapPixel);

	CAM->setRange(_TownMap->getWidth(), _TownMap->getHeight());
	CAM->setPosition(_player->getX(), _player->getY());


	//SOUNDMANAGER->play("����1");
	portal = RectMakeCenter(4000, 7500, 100, 100);

	temp.init();
	temp.setX(_player->getX());
	temp.setY(_player->getY());

	cloud *temp;
	for (int i = 0; i < 70; ++i)
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

	return S_OK;
}

void townScene::release()
{
	RENDERMANAGER->clearBackground();
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
	}
	if (temp.getState() == HANG)
		temp.lifted(_player);

	if (KEYMANAGER->isOnceKeyDown('2'))
	{
		if (temp.getState() == HANG)
		{
			temp.throwed(10, getAngle(temp.getX() - CAM->getX(), temp.getY() - CAM->getY(), _ptMouse.x, _ptMouse.y));
		}
	}
	RECT temp;
	if (IntersectRect(&temp, &_player->getHitbox(), &portal))
	{
		SCENEMANAGER->loadScene("dungeonScene");
	}
	

	/*����Ʈ �Ŵ��� ����*/
	if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
	{
		EFFECTMANAGER->play("����", _ptMouse.x, _ptMouse.y);
	}

	this->temp.update();

	CAM->videoShooting(_player->getX(), _player->getY());
}

void townScene::render()
{
	RENDERMANAGER->backgroundRender(getMemDC());

	//_testMap->render(getMemDC());
	_TownMap->render(getMemDC(), CAM->getSX(), CAM->getSY(), CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY);
	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		_TownMapPixel->render(getMemDC(), CAM->getSX(), CAM->getSY(), CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY);
		Rectangle(getMemDC(), portal.left - CAM->getX(), portal.top - CAM->getY(), portal.right - CAM->getX(), portal.bottom - CAM->getY());
	}
	_player->render();

	RENDERMANAGER->addRender(0, &temp);
}
