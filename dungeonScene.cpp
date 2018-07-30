#include "stdafx.h"
#include "dungeonScene.h"

HRESULT dungeonScene::init()
{
	_player = SAVEDATA->getPlayer();
	_player->init();
	_player->setX(1000);
	_player->setY(500);

	_stage = IGM->findImage("������1");
	_stagePixel = IMAGEMANAGER->findImage("������1�ȼ�");
	_player->setMap(_stage);
	_player->setMapPixel(_stagePixel);
	
	_tortoise = new tortoise;
	_shield = new tortoiseShield; //_tortoise->setShieldLink�� ����ϹǷ� �� ��ġ���� �����Ҵ�

	_tortoise->setPlayerLink(_player);
	_tortoise->setShieldLink(_shield);
	_tortoise->init(1400,850); //init�Լ� �ȿ� setLink���� �����Ƿ� ���� 2�� _tortoise->setLink���� �ؾ���

	_shield->init(_tortoise->getX(), _tortoise->getY()); //_tortoise�� x,y�� �޾Ƽ� init�ؾ��ϹǷ� �� ������

	_gawk1 = new gawk;
	_gawk1->init(1200, 300);

	CAM->init();
	CAM->setRange(_stage->getWidth(), _stage->getHeight());
	CAM->setPosition(_player->getX(), _player->getY());


	return S_OK;
}

void dungeonScene::update()
{
	_tortoise->update();
	_shield->update();
	_gawk1->update();
	_player->update();

	CAM->videoShooting(_player->getX(), _player->getY());
}

void dungeonScene::render()
{
	_stage->render(getMemDC(), 0, 0, CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY);
	_tortoise->render();
	_shield->render();
	_gawk1->render();
	_player->render();
}

void dungeonScene::release()
{
	_tortoise->release();
	SAFE_DELETE(_tortoise);
	_gawk1->release();
	SAFE_DELETE(_gawk1);
	_shield->release();
	SAFE_DELETE(_shield);
}
