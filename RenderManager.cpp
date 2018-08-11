#include "stdafx.h"
#include "RenderManager.h"
#include "actor.h"
#include "backgroundActor.h"

HRESULT RenderManager::init()
{
	return S_OK;
}

void RenderManager::release()
{
	renderList.clear();
}

void RenderManager::render(HDC hdc)
{
	multimap<float, actor *>::iterator iter;
	int x, y;
	if (!renderList.empty())
	{
		for (iter = renderList.begin(); iter != renderList.end(); ++iter)
		{
			x = iter->second->getX() - CAM->getX();
			y = iter->second->getY() - CAM->getY();
			if (0 < x + 200 && x - 200 < WINSIZEX &&
				0 < y + 200 && y - 200 < WINSIZEY)
			{
				iter->second->render();
			}
		}
	}

	renderList.clear();
}

void RenderManager::backgroundRender(HDC hdc)
{
	multimap<float, backgroundActor *>::iterator iter;
	float centerX = CAM->getX() + WINSIZEX / 2, centerY = CAM->getY() + WINSIZEY / 2;
	float x, y, z;
	float width, height;
	if (!backgroundList.empty())
	{
		for (iter = backgroundList.begin(); iter != backgroundList.end(); ++iter)
		{
			iter->second->update();
			width = iter->second->getImage()->getWidth();
			height = iter->second->getImage()->getHeight();
			z = iter->second->getZ();

			x = WINSIZEX / 2 + (iter->second->getX() - centerX) / (5 - z);
			y = WINSIZEY / 2 + (iter->second->getY() - centerY) / (5 - z);
			if (0 < x + width && x < WINSIZEX &&
				0 < y + height && y < WINSIZEY)
			{
				iter->second->render(x, y);
			}
		}
	}

}

void RenderManager::addRender(float z, actor *name)
{
	renderList.insert(make_pair(z, name));
}

void RenderManager::addBackground(float z, backgroundActor *backActor)
{
	backgroundList.insert(make_pair(z, backActor));
}

void RenderManager::clearBackground()
{
	multimap<float, backgroundActor *>::iterator iter;
	if (!backgroundList.empty())
	{
		for (iter = backgroundList.begin(); iter != backgroundList.end(); ++iter)
		{
			SAFE_DELETE(iter->second);
		}
	}

	backgroundList.clear();
}
