#include "stdafx.h"
#include "stuffManager.h"
#include "player.h"

HRESULT stuffManager::init()
{
	return S_OK;
}

void stuffManager::release()
{
	for (int i = 0; i < _vStuff.size(); ++i)
	{
		SAFE_DELETE(_vStuff[i]);
	}
	for (int i = 0; i < _vFruit.size(); ++i)
	{
		SAFE_DELETE(_vFruit[i]);
	}
}

void stuffManager::update()
{
	for (int i = 0; i < _vStuff.size(); ++i)
	{
		_vStuff[i]->update();
	}
	for (int i = 0; i < _vFruit.size(); ++i)
	{
		_vFruit[i]->update();
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

void stuffManager::addFruit(int type, float x, float y)
{
	liftableActor * temp;
	temp = new fruit();
	temp->init();
	temp->setX(x);
	temp->setY(y);
}

void stuffManager::addStuff(int type, float x, float y)
{
	liftableActor * temp;
	temp = new ironWeight;
	temp->init();
	temp->setX(x);
	temp->setY(y);
}

void stuffManager::removeFruit(int index)
{
}

stuffManager::stuffManager()
{
}


stuffManager::~stuffManager()
{
}
