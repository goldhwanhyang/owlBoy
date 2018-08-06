#include "stdafx.h"
#include "enemyManager.h"
#include "player.h"

HRESULT enemyManager::init()
{
	initTortoise();

	_factory = new enemyFactory;

	for (int i = 0; i < 3; ++i)
	{
		enemy* _enemy = _factory->createEnemy(GAWK);
		_enemy->setPlayerLink(_player);
		_vEnemy.push_back(_enemy);
	}
	_vEnemy[2]->setPostion(1200, 130, 1);
	_vEnemy[3]->setPostion(900, 130, 0);
	_vEnemy[4]->setPostion(400, 130, 0);

	for (int i = 0; i < 2; ++i)
	{
		enemy* _enemy = _factory->createEnemy(TORQUE);
		_enemy->setPlayerLink(_player);
		_vEnemy.push_back(_enemy);
	}
	_vEnemy[5]->setPostion(800, 940, 0);
	_vEnemy[6]->setPostion(400, 940, 1);

	return S_OK;
}

void enemyManager::update()
{
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		if (!_vEnemy[i]->getIsActive()) continue;

		_vEnemy[i]->update();
	}
}

void enemyManager::render()
{
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		if (!_vEnemy[i]->getIsActive()) continue;

		_vEnemy[i]->render();
	}
}

void enemyManager::release()
{
	SAFE_DELETE(_factory);

	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->release();
		SAFE_DELETE(_vEnemy[i]);
	}
}

void enemyManager::initTortoise()
{
	tortoise * _tortoise;
	_tortoise = new tortoise;
	_shield = new tortoiseShield; //_tortoise->setShieldLink를 써야하므로 이 위치에서 동적할당

	_tortoise->setPlayerLink(_player);
	_tortoise->setShieldLink(_shield);
	_tortoise->init(1400, 850); //init함수 안에 setLink들이 있으므로 위의 2줄 _tortoise->setLink부터 해야함

	_shield->init(_tortoise->getX(), _tortoise->getY()); //_tortoise의 x,y를 받아서 init해야하므로 맨 마지막

	_vEnemy.push_back(_tortoise);
	_vEnemy.push_back(_shield);
}

enemy * enemyFactory::createEnemy(int type)
{
	enemy* _enemy;
	switch (type)
	{
	case GAWK:
		_enemy = new gawk;
		break;
	case TORQUE:
		_enemy = new torque;
		break;
	}

	_enemy->init();

	return _enemy;
}
