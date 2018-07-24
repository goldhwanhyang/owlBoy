#include "stdafx.h"
#include "dungeonScene.h"

HRESULT dungeonScene::init()
{
	_stage = IGM->addImage("º¸½º¹æ1", 1920, 1080);
	CAM->setRange(_stage->getWidth(), _stage->getHeight());
	return S_OK;
}

void dungeonScene::update()
{
}

void dungeonScene::render()
{
	_stage->render(getMemDC(), WINSIZEX - CAM->getX(), WINSIZEY - CAM->getY());
}

void dungeonScene::release()
{
}
