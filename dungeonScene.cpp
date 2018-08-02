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

	enemy* Gawk;
	for (int i = 0; i < 2; i++)
	{
		Gawk = new gawk;
		Gawk->setPlayerLink(_player);
		Gawk->init();
		_vEnemy.push_back(Gawk);
	}
	_vEnemy[0]->setPostion(1200, 500, 1);
	_vEnemy[1]->setPostion(900, 500, 0);

	CAM->init();
	CAM->setRange(_stage->getWidth(), _stage->getHeight());
	CAM->setPosition(_player->getX(), _player->getY());


	return S_OK;
}

void dungeonScene::update()
{
	_tortoise->update();
	_shield->update();
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->update();
	}
	_player->update();

	CAM->videoShooting(_player->getX(), _player->getY());
}

void dungeonScene::render()
{
	_stage->render(getMemDC(), CAM->getSX(), CAM->getSY(), CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY); //ī�޶� ����ũ�� �̹����� CAM->getSX(), CAM->getSY()
	_tortoise->render();
	_shield->render();
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		//_vEnemy[i]->render();
	}
	_vEnemy[0]->render();
	_player->render();
}

void dungeonScene::release()
{
	_tortoise->release();
	SAFE_DELETE(_tortoise);
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->release();
		SAFE_DELETE(_vEnemy[i]);
	}
	_shield->release();
	SAFE_DELETE(_shield);
}
