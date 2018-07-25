#include "stdafx.h"
#include "player.h"

HRESULT player::init()
{
	_testMap = IMAGEMANAGER->addImage("map", "Texture/player/1000x1000_map.bmp", 1000, 1000, true, RGB(255, 0, 255));
	CAM->setRange(_testMap->getWidth(), _testMap->getHeight());

	img[IDLE] = IMAGEMANAGER->addFrameImage("IDLE", "Texture/player/test_otusIdle_350x125_10x2.bmp", 350, 125, 10, 2);
	img[WALK] = IMAGEMANAGER->addFrameImage("Walk", "Texture/player/test_otusWalk_500x135_8x2.bmp", 500, 135, 8, 2);
	img[JUMP] = IMAGEMANAGER->addFrameImage("test", "Texture/player/test_otusJump_180x130_3x2.bmp", 180, 130, 3, 2);
	img[JUMPFALL] = IMAGEMANAGER->addFrameImage("fall", "Texture/player/test_otusJumpFall_350x140_5x2.bmp", 350, 140, 5, 2);
	_isLeft = false;	// 왼쪽 오른쪽
	_isWalk = false;	
	_isJump = false;
	_x = 640.f;			// 플레이어 x좌표
	_y = 465.f;			// 플레이어 y좌표
	_speed = 6.0f;		// 플레이어 속도
	_angle = PI / 2;
	_gravity = 0;		// 플레이어 중력
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH); 
	_count = _index = 0;	// 프레임이미지 돌리기 위한 초기화
	_weight = 0.f;
	_state = IDLE;

	return S_OK;
}

void player::release()
{

}

void player::update()
{
	if (KEYMANAGER->isStayKeyDown('A'))
	{
		_x -= _speed;
		_isLeft = true;
		_state = WALK;
	}
	else
	{
		_state = IDLE;
	}
	if (KEYMANAGER->isStayKeyDown('D'))
	{
		_x += _speed;
		_isLeft = false;
		_state = WALK;
	}
	if (KEYMANAGER->isStayKeyDown('W') )
	{
		if (_isJump == false)
		{
			_y -= _speed;
		}
		_weight -= 0.60f;
		_isJump = true;
		//_state = JUMP;		
	}
	if (KEYMANAGER->isStayKeyDown('S'))
	{
		_y += _speed;
	}

	_gravity += 0.68f;
	if (_isJump == true)
	{
		_state = JUMP;
		_y += -sinf(_angle) * _speed + _gravity + _weight;
	}
	if (_isJump == false)
		_gravity = 0.f;
	//	_y += _gravity;

	{
		COLORREF color = GetPixel(_testMap->getMemDC(), _x, _hitBox.bottom);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// 마젠타가 아니면 검사
		{
			if (_y = _hitBox.bottom - 30)
			{
				//_gravity = 0.f;
				_weight = 0.f;
				_isJump = false;
			}

			//break;
		}
	}

	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);
	//아울보이 프레임 돌리기
	_count++;
	if (_isLeft == false)
	{
		img[_state]->setFrameY(0);
		if (_count % 7 == 0)
		{
			_index++;
			if (_index > img[_state]->getMaxFrameX())
			{
				_index = 0;
			}
			img[_state]->setFrameX(_index);
		}
	}
	if (_isLeft == true)
	{
		img[_state]->setFrameY(1);
		if (_count % 7 == 0)
		{
			_index--;
			if (_index < 0)
			{
				_index = img[_state]->getMaxFrameX();
			}
			img[_state]->setFrameX(_index);
		}
	}
	//위에 검사
	//for (int i = _hitBox.top; i < _hitBox.top + 1; i++)
	{
		COLORREF color = GetPixel(_testMap->getMemDC(), _x, _hitBox.top);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_y = _hitBox.top + 30;
			//break;
		}
	}
	//아래 검사
	//for (int i = _hitBox.bottom; i < _hitBox.bottom+1; i++)
	{
		COLORREF color = GetPixel(_testMap->getMemDC(), _x, _hitBox.bottom);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);
	
		if (!(r == 255 && g == 0 && b == 255))	// 마젠타가 아니면 검사
		{
			_y = _hitBox.bottom-30;
			//break;
		}
	}
	//왼쪽 검사
	//for (int i = _hitBox.left; i < _hitBox.left + 1; i++)
	{
		COLORREF color = GetPixel(_testMap->getMemDC(), _hitBox.left, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// 마젠타가 아니면 검사
		{
			_x = _hitBox.left + 20;
			//break;
		}
	}
	//오른쪽 검사
	//for (int i = _hitBox.right; i < _hitBox.right + 1; i++)
	{
		COLORREF color = GetPixel(_testMap->getMemDC(), _hitBox.right, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// 마젠타가 아니면 검사
		{
			_x = _hitBox.right - 20;
			//break;
		}
	}
}

void player::render()
{
	_testMap->render(getMemDC());
	RECT topRc = RectMakeCenter(_x, _hitBox.top, 10, 10);
	//Rectangle(getMemDC(), topRc);
	RECT BottomRc = RectMakeCenter(_x, _hitBox.bottom, 10, 10);	// 감지 
	//Rectangle(getMemDC(), BottomRc);
	Rectangle(getMemDC(), _hitBox);
	
//	img->frameRender(getMemDC(), _x-img->getFrameWidth()/2, _y-img->getFrameHeight()/2);
	img[_state]->frameRender(getMemDC(), _x - img[_state]->getFrameWidth() / 2, _y - img[_state]->getFrameHeight() / 2);
	char str[128];
	sprintf_s(str, "%.3f %.3f %.3f %.3f", _x, _y,_gravity, _weight);
	TextOut(getMemDC(), 10, 10, str, strlen(str));
	//sprintf_s(str, "%d %d", _count, _index);
}

