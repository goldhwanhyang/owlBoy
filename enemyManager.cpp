#include "stdafx.h"
#include "enemyManager.h"
#include "player.h"	// 전방선언한 player클래스는 여기서 가져다 사용해라. include안해주면 플레이어에있는 멤버를 전혀 사용할 수 없다.


HRESULT enemyManager::init()
{
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

		RENDERMANAGER->addRender(_vEnemy[i]->getZ(), _vEnemy[i]);
	}
}

void enemyManager::release()
{
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->release();
		SAFE_DELETE(_vEnemy[i]);
	}
}

void enemyManager::setPlayer(player * p)
{
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->setPlayerLink(p);
	}
}

void enemyManager::setMapPixel(image * map)
{
	for (int i = 0; i < _vEnemy.size(); i++)
	{
		_vEnemy[i]->setMapPixel(map);
	}
}

enemy * enemyFactory::createEnemy(int type, float x, float y, int dir)
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

	_enemy->init(x,y,dir);
	return _enemy;
}
