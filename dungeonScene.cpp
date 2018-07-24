#include "stdafx.h"
#include "dungeonScene.h"

HRESULT dungeonScene::init()
{
	_boss1 = new boss1;
	_boss1->init(1400, 850);

	_stage = IGM->addImage("º¸½º¹æ1","Texture/Maps/Boss1/Map.bmp", 1920, 1080);
	CAM->setRange(_stage->getWidth(), _stage->getHeight());
	return S_OK;
}

void dungeonScene::update()
{
	_boss1->update();
}

void dungeonScene::render()
{
	_stage->render(getMemDC());
	_boss1->render();
}

void dungeonScene::release()
{
	_boss1->release();
	SAFE_DELETE(_boss1);
}
