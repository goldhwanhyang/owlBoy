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
		_enemy->setMapPixel(_mapPixel);
		_vEnemy.push_back(_enemy);
	}
	_vEnemy[2]->setPostion(1200, 130, 1);
	_vEnemy[3]->setPostion(900, 130, 0);
	_vEnemy[4]->setPostion(400, 130, 0);

	for (int i = 0; i < 2; ++i)
	{
		enemy* _enemy = _factory->createEnemy(TORQUE);
		_enemy->setPlayerLink(_player);
		_enemy->setMapPixel(_mapPixel);
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
	_shield = new tortoiseShield; //_tortoise->setShieldLink�� ����ϹǷ� �� ��ġ���� �����Ҵ�

	_tortoise->setPlayerLink(_player);
	_tortoise->setMapPixel(_mapPixel);
	_tortoise->setShieldLink(_shield);
	_tortoise->init(1400, 850); //init�Լ� �ȿ� setLink���� �����Ƿ� ���� 2�� _tortoise->setLink���� �ؾ���

	_shield->init(_tortoise->getX(), _tortoise->getY()); //_tortoise�� x,y�� �޾Ƽ� init�ؾ��ϹǷ� �� ������

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
