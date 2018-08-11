#include "stdafx.h"
#include "stuffManager.h"
#include "player.h"
#include "enemyManager.h"

HRESULT stuffManager::init()
{
	return S_OK;
}

void stuffManager::release()
{
	for (int i = 0; i < _vStuff.size(); ++i)
	{
		_vStuff[i]->release();
		SAFE_DELETE(_vStuff[i]);
	}
	for (int i = 0; i < _vFruit.size(); ++i)
	{
		_vFruit[i]->release();
		SAFE_DELETE(_vFruit[i]);
	}
}

void stuffManager::update()
{
	vector<enemy *> em = _enemyManager->getVEnemy();
	for (int i = 0; i < _vStuff.size(); ++i)
	{
		if (_vStuff[i]->getIsActive())
		{
			_vStuff[i]->update();

			if (_vStuff[i]->getState() != ON_AIR) continue;

			for (int j = 0; j < em.size(); ++j)
			{
				if (_vStuff[i]->collide(em[j]))
				{
					em[j]->damaged(_vStuff[i]);
					break;
				}
			}
		}
	}
	for (int i = 0; i < _vFruit.size(); ++i)
	{
		if (_vFruit[i]->getIsActive())
		{
			_vFruit[i]->update();
		}
	}
}

void stuffManager::render()
{
	for (int i = 0; i < _vStuff.size(); ++i)
	{
		if (_vStuff[i]->getIsActive())
			RENDERMANAGER->addRender(_vStuff[i]->getZ(), _vStuff[i]);
	}
	for (int i = 0; i < _vFruit.size(); ++i)
	{
		if(_vFruit[i]->getIsActive())
			RENDERMANAGER->addRender(_vFruit[i]->getZ(), _vFruit[i]);
	}
}

liftableActor* stuffManager::collide(player * _player)
{
	for (int i = 0; i < _vStuff.size(); ++i)
	{
		if (_vStuff[i]->getIsActive() && _vStuff[i]->collide(_player))
		{
			return _vStuff[i];
		}
	}
	for (int i = 0; i < _vFruit.size(); ++i)
	{
		if (_vFruit[i]->getIsActive() && _vFruit[i]->collide(_player))
		{
			return _vFruit[i];
		}
	}
	return NULL;
}


void stuffManager::addFruit(int type, float x, float y)
{
	liftableActor * temp;
	temp = new fruit();
	temp->init();
	temp->setX(x);
	temp->setY(y);

	_vFruit.push_back(temp);
}

void stuffManager::addStuff(int type, float x, float y)
{
	liftableActor * temp;
	temp = new ironWeight;
	temp->init();
	temp->setX(x);
	temp->setY(y);

	_vStuff.push_back(temp);
}

void stuffManager::addStuff(int type, float x, float y, image * mapPixel)
{
	liftableActor * temp;
	temp = new ironWeight;
	temp->init();
	temp->setX(x);
	temp->setY(y);
	temp->setMapPixel(mapPixel);

	_vStuff.push_back(temp);
}

void stuffManager::addFruit(int type, float x, float y, image * mapPixel)
{

	liftableActor * temp;
	temp = new fruit();
	temp->init();
	temp->setX(x);
	temp->setY(y);
	temp->setMapPixel(mapPixel);

	_vFruit.push_back(temp);
}

void stuffManager::removeFruit(int index)
{

}
