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

	SOUNDMANAGER->addSound("����1", "���.mp3");
	SOUNDMANAGER->addSound("����2", "���1.mp3");

	SOUNDMANAGER->play("����1");
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
		if (SOUNDMANAGER->isPlaySound("����1"))
		{
			SOUNDMANAGER->stop("����1");
			SOUNDMANAGER->play("����2");
		}
	}

	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		SOUNDMANAGER->pause("����2");
	}
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		SOUNDMANAGER->resume("����2");
	}
}

void playingScene::render()
{
	//_testMap->render(getMemDC());
	_player->render();
}
