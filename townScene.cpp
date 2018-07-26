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


	SOUNDMANAGER->play("사운드1");
	portal = RectMakeCenter(4000, 7500, 100, 100);

	return S_OK;
}

void townScene::release()
{
}

void townScene::update()
{
	_player->update();

	if (KEYMANAGER->isOnceKeyDown('1'))
	{
		if (SOUNDMANAGER->isPlaySound("사운드1"))
		{
			SOUNDMANAGER->stop("사운드1");
			SOUNDMANAGER->play("사운드2");
		}
	}

	if (KEYMANAGER->isOnceKeyDown('2'))
	{
		SOUNDMANAGER->pause("사운드2");
	}
	if (KEYMANAGER->isOnceKeyDown('3'))
	{
		SOUNDMANAGER->resume("사운드2");
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

	CAM->videoShooting(_player->getX(), _player->getY());
}

void townScene::render()
{
	//_testMap->render(getMemDC());
	_TownMap->render(getMemDC(), CAM->getSX(), CAM->getSY(), CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY);
	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		_TownMapPixel->render(getMemDC(), CAM->getSX(), CAM->getSY(), CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY);
		Rectangle(getMemDC(), portal.left - CAM->getX(), portal.top - CAM->getY(), portal.right - CAM->getX(), portal.bottom - CAM->getY());
	}
	_player->render();


}
