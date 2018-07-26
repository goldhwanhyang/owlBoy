#include "stdafx.h"
#include "dungeonScene.h"

HRESULT dungeonScene::init()
{
	_player = SAVEDATA->getPlayer();
	_player->init();
	_player->setX(1000);
	_player->setY(500);

	_stage = IGM->findImage("보스방1");
	_stagePixel = IMAGEMANAGER->findImage("보스방1픽셀");
	_player->setMap(_stage);
	_player->setMapPixel(_stagePixel);

	_boss1 = new boss1;
	_boss1->init(1400, 850);

	_gawk1 = new gawk;
	_gawk1->init(1200, 300);

	CAM->init();
	CAM->setRange(_stage->getWidth(), _stage->getHeight());
	CAM->setPosition(_player->getX(), _player->getY());

	_boss1->setPlayerLink(_player);
	return S_OK;
}

void dungeonScene::update()
{
	_boss1->update();
	_gawk1->update();
	_player->update();

	CAM->videoShooting(_player->getX(), _player->getY());
}

void dungeonScene::render()
{
	_stage->render(getMemDC(), 0, 0, CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY);
	_boss1->render();
	_gawk1->render();
	_player->render();
}

void dungeonScene::release()
{
	_boss1->release();
	SAFE_DELETE(_boss1);
	_gawk1->release();
	SAFE_DELETE(_gawk1);


}
