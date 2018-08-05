#include "stdafx.h"
#include "startScene.h"


HRESULT startScene::init()
{
	_bgImage = IMAGEMANAGER->findImage("START_SCENE_BACKGROUND");
	_bgCloud = IMAGEMANAGER->findImage("START_SCENE_BACKGROUND_CLOUD");
	_stone1 = IMAGEMANAGER->findImage("START_SCENE_OTUS");
	_stone2 = IMAGEMANAGER->findImage("START_SCENE_STONE");

	_selectedAlpha = 255;
	_nonAlpha = 180;
	_count = _index = 0;
	_angle = 0;

	return S_OK;
}

void startScene::release()
{
}

void startScene::update()
{
	_angle += PI / 5000;
	_count = (_count + 1) % 10;
	if (_count == 0)
	{
		_index = (_index + 1) % (_stone1->getMaxFrameX() + 1);
	}

	if (_angle > 2 * PI) _angle -= 2 * PI;

	if (KEYMANAGER->isOnceKeyDown(' '))
	{
		SCENEMANAGER->loadScene("townScene");
	}
}

void startScene::render()
{
	_bgImage->render(getMemDC());
	_bgCloud->rotateRender(getMemDC(), WINSIZEX / 2, WINSIZEY / 2 - 500, _angle);
	_stone2->render(getMemDC(), WINSIZEX - _stone2->getWidth(), WINSIZEY - _stone2->getHeight());
	_stone1->frameRender(getMemDC(), 0, WINSIZEY - _stone1->getFrameHeight(), _index, 0);
}
