#include "stdafx.h"
#include "enemyManager.h"

HRESULT enemyManager::init()
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

	enemy* _gawk;
	for (int i = 0; i < 2; i++)
	{
		_gawk = new gawk;
		_gawk->setPlayerLink(_player);
		_gawk->init();
		_vEnemy.push_back(_gawk);
	}
	_vEnemy[2]->setPostion(1200, 500, 1);
	_vEnemy[3]->setPostion(900, 500, 0);

	enemy* _torque = new torque;
	_torque->setPlayerLink(_player);
	_torque->init(800,800);
	_vEnemy.push_back(_torque);

	return S_OK;
}

void enemyManager::update()
{
	//_tortoise->update();
	//_shield->update();
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->update();
	}
}

void enemyManager::render()
{
	//_tortoise->render();
	//_shield->render();
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->render();
	}
}

void enemyManager::release()
{
	//_tortoise->release();
	//SAFE_DELETE(_tortoise);
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->release();
		SAFE_DELETE(_vEnemy[i]);
	}
	//_shield->release();
	//SAFE_DELETE(_shield);
}