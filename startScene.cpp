#include "stdafx.h"
#include "startScene.h"

float _soundVolume = 1;
float _effectVolume = 1;

HRESULT startScene::init()
{
	_bgImage = IMAGEMANAGER->findImage("START_SCENE_BACKGROUND");
	_bgCloud = IMAGEMANAGER->findImage("START_SCENE_BACKGROUND_CLOUD");
	_stone1 = IMAGEMANAGER->findImage("START_SCENE_OTUS");
	_stone2 = IMAGEMANAGER->findImage("START_SCENE_STONE");

	_player= SAVEDATA->getPlayer();
	_player->init();
	// 마을 씬의 좌표
	_player->setX(1963.f);
	_player->setY(7121.f);

	_maxOption = 3;

	_selectedAlpha = 255;
	_nonAlpha = 180;
	_count = _index = 0;
	_angle = 0;
	_curMenu = 0;
	_selectedOption = -1;

	_title = IMAGEMANAGER->findImage("TITLE");

	_text[0] = IMAGEMANAGER->findImage("CONTINUE");
	_text[2] = IMAGEMANAGER->findImage("GAME_EXIT");
	_text[1] = IMAGEMANAGER->findImage("OPTION");
	
	_backToMenu = IMAGEMANAGER->findImage("BACK_TO_MENU");
	_board = IMAGEMANAGER->findImage("VOLUME_SETTING");

	_title->setX(WINSIZEX / 2 - _title->getWidth() / 2);
	_title->setY(50);
	int y = _title->getHeight() + _title->getY();
	for (int i = 0; i < _maxOption; ++i)
	{
		_text[i]->setX(WINSIZEX / 2 - _text[i]->getWidth() / 2);
		_text[i]->setY(y);
		_text[i]->alphaRender(getMemDC(), _selectedAlpha);
		y += 100;
	}


	_board->setX(WINSIZEX / 2 - _board->getWidth() / 2);
	_board->setY(WINSIZEY / 2 - _board->getHeight() / 2);

	_backToMenu->setX(_board->getX());
	_backToMenu->setY(_board->getY() - _backToMenu->getHeight() - 50);

	_volume = new progressBar;
	_volumeX = WINSIZEX / 2 - 35;
	_volumeY = WINSIZEY / 2 - _board->getY() / 4 - 65;
	_volume->init("Texture/UI/volumeProgressFront_641x66", "Texture/UI/volumeProgressBack_673x81", _volumeX ,_volumeY, _board->getWidth() / 2 - 25, 50);
	_volume->setGauge(_soundVolume * 100, 100);
	_volumeWidth = 0;

	_effectVolumeBar = new progressBar;
	_effectVolumeX = WINSIZEX / 2 - 35;
	_effectVolumeY = WINSIZEY / 2 - _board->getY() / 4 + 35;
	_effectVolumeBar->init("Texture/UI/volumeProgressFront_641x66", "Texture/UI/volumeProgressBack_673x81", _effectVolumeX, _effectVolumeY, _board->getWidth() / 2 - 25, 50);
	_effectVolumeBar->setGauge(_effectVolume * 100, 100);
	_effectVolumeWidth = 0;



	ShowCursor(false);

	SOUNDMANAGER->playBgm("시작", _soundVolume);


	return S_OK;
}

void startScene::release()
{
	_volume->release();
	SAFE_DELETE(_volume);
	_effectVolumeBar->release();
	SAFE_DELETE(_effectVolumeBar);
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


	switch (_curMenu)
	{
	case 0:
		updateStartMenu();
		break;
	case 1:
		updateOption();
		break;
	}
	
}

void startScene::render()
{
	_bgImage->render(getMemDC());
	_bgCloud->rotateRender(getMemDC(), WINSIZEX / 2, WINSIZEY / 2 - _bgCloud->getHeight()/2, _angle);
	_stone2->render(getMemDC(), WINSIZEX - _stone2->getWidth(), WINSIZEY - _stone2->getHeight());
	_stone1->frameRender(getMemDC(), 0, WINSIZEY - _stone1->getFrameHeight(), _index, 0);

	switch (_curMenu)
	{
	case 0:
		drawStartMenu();
		break;
	case 1:
		drawOption();
		break;
	}
}

void startScene::updateStartMenu()
{

	_selectedOption = -1;
	for (int i = 0; i < _maxOption; ++i)
	{
		if (PtInRect(&_text[i]->boundingBox(), _ptMouse))
		{
			_selectedOption = i;
		}
	}

	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		if (_selectedOption == -1)
		{
			return;
		}

		switch (_selectedOption)
		{
		case 0:
			SOUNDMANAGER->play("게임시작", 1);
			_player->setX(1963.f);
			_player->setY(7121.f);
			SCENEMANAGER->loadScene("townScene");
			break;
		case 1:
			_curMenu = 1;
			SOUNDMANAGER->play("메뉴선택", 1);
			break;
		case 2:
			SOUNDMANAGER->play("메뉴선택", 1);
			PostMessage(_hWnd, WM_DESTROY, 0, 0);
			break;
		}
	}
}

void startScene::updateOption()
{
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		if (PtInRect(&_backToMenu->boundingBox(), _ptMouse))
		{
			SOUNDMANAGER->play("메뉴선택", 1);
			_curMenu = 0;
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		RECT temp = _volume->getRect();
		if (PtInRect(&temp, _ptMouse))
		{
			_soundVolume = (_ptMouse.x - temp.left) / (float)(temp.right - temp.left);
			_volume->setGauge(_soundVolume * 100, 100);
			SOUNDMANAGER->setVolume(_soundVolume);
		}

		temp = _effectVolumeBar->getRect();
		if (PtInRect(&temp, _ptMouse))
		{
			_effectVolume = (_ptMouse.x - temp.left) / (float)(temp.right - temp.left);
			_effectVolumeBar->setGauge(_effectVolume * 100, 100);
		}

		
	}
	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
	{
		RECT temp = _effectVolumeBar->getRect();
		if (PtInRect(&temp, _ptMouse))
		{
			SOUNDMANAGER->play("공격", _effectVolume);
		}
	}

	_volume->update();
	_effectVolumeBar->update();
}

void startScene::drawOption()
{
	_board->render(getMemDC(), _board->getX(), _board->getY());
	_volume->render();
	_effectVolumeBar->render();
	_backToMenu->render(getMemDC(), _backToMenu->getX(), _backToMenu->getY());
}

void startScene::drawStartMenu()
{
	_title->render(getMemDC(), _title->getX(), _title->getY());

	for (int i = 0; i < _maxOption; ++i)
	{
		
		if (i == _selectedOption)
		{
			_text[i]->render(getMemDC(), _text[i]->getX(), _text[i]->getY());
		}
		else
		{
			_text[i]->alphaRender(getMemDC(), 100);
		}
	}
}
