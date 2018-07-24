#include "stdafx.h"
#include "playingScene.h"


HRESULT playingScene::init()
{
	//_testMap = IMAGEMANAGER->addImage("map", "Texture/player/1000x1000_map.bmp", 1000, 1000, true, RGB(255, 0, 255));

	CAM->init();
	//CAM->setRange(_testMap->getWidth(), _testMap->getHeight());

	_player = new player;
	_player->init();
	CAM->setPosition(_player->getX(), _player->getY());

	SOUNDMANAGER->addSound("사운드1", "브금.mp3");
	SOUNDMANAGER->addSound("사운드2", "브금1.mp3");

	SOUNDMANAGER->play("사운드1");
	return S_OK;
}

void playingScene::release()
{
	_player->release();
	SAFE_DELETE(_player);
}

void playingScene::update()
{
	_player->update();

	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		if (SOUNDMANAGER->isPlaySound("사운드1"))
		{
			SOUNDMANAGER->stop("사운드1");
			SOUNDMANAGER->play("사운드2");
		}
	}

	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		SOUNDMANAGER->pause("사운드2");
	}
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		SOUNDMANAGER->resume("사운드2");
	}
}

void playingScene::render()
{
	//_testMap->render(getMemDC());
	_player->render();
}
