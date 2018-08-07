#include "stdafx.h"
#include "storeScene.h"


HRESULT storeScene::init()
{
	//_testMap = IMAGEMANAGER->addImage("map", "Texture/player/1000x1000_map.bmp", 1000, 1000, true, RGB(255, 0, 255));
	_player = SAVEDATA->getPlayer();
	_player->init();

	//CAM->setRange(_testMap->getWidth(), _testMap->getHeight());

	_storeMap = IMAGEMANAGER->findImage("Town");
	_storeMapPixel = IMAGEMANAGER->findImage("TownPixel");

	return S_OK;
}

void storeScene::release()
{
}

void storeScene::update()
{
}

void storeScene::render()
{
}

storeScene::storeScene()
{
}


storeScene::~storeScene()
{
}
