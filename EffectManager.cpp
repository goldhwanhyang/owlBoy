#include "stdafx.h"
#include "EffectManager.h"


HRESULT EffectManager::init()
{
	return S_OK;
}

void EffectManager::release()
{
	list<Effect>::iterator iter = effectList.begin();

	effectList.clear();
}


void EffectManager::render(HDC hdc)
{
	list<Effect>::iterator iter;

	if (!effectList.empty())
	{
		iter = effectList.begin();
		while( iter != effectList.end())
		{
			iter->count = (iter->count + 1) % 10;
			if (iter->count == 0)
			{
				iter->index++;
				if (iter->index > iter->eImage->getMaxFrameX())
				{
					iter = effectList.erase(iter);
					continue;
				}
			}
			iter->eImage->frameRender(hdc, iter->x - CAM->getX() - iter->eImage->getFrameWidth()/2,
				iter->y - CAM->getY() - iter->eImage->getFrameHeight()/2, iter->index, iter->frameY);

			++iter;
		}
	}
}



void EffectManager::generateEffect(const char * effectName, float x, float y, int frameY)
{
	Effect e;
	e.eImage = IMAGEMANAGER->findImage(effectName);
	e.x = x;
	e.y = y;
	e.index = e.count = 0;
	e.frameY = frameY;

	effectList.push_back(e);
}



EffectManager::EffectManager()
{
}


EffectManager::~EffectManager()
{
}
