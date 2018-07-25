#include "stdafx.h"
#include "player.h"

HRESULT player::init()
{
	//_testMap = IMAGEMANAGER->addImage("map", "Texture/player/1000x1000_map.bmp", 1000, 1000, true, RGB(255, 0, 255));

	img[IDLE] = IMAGEMANAGER->findImage("IDLE");
	img[WALK] = IMAGEMANAGER->findImage("Walk");
	img[JUMP] = IMAGEMANAGER->findImage("Jump");
	img[JUMPFALL] = IMAGEMANAGER->findImage("Fall");
	img[FLY] = IMAGEMANAGER->findImage("FLY");

	_isLeft = false;	// 왼쪽 오른쪽
	_isWalk = false;	
	_isJump = false;

	_x = 540.f;			// 플레이어 x좌표
	_y = 465.f;			// 플레이어 y좌표
	_speed = 6.0f;		// 플레이어 속도
	_angle = PI / 2;
	_gravity = 0;		// 플레이어 중력
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH); 
	_count = _index = 0;	// 프레임이미지 돌리기 위한 초기화

	_state = IDLE;

	return S_OK;
}

void player::release()
{

}

void player::update()
{
	if (_isJump == false)
	{
		_state = IDLE;
	}
	if (KEYMANAGER->isStayKeyDown('A'))
	{
		_x -= _speed;
		_isLeft = true;
		_state = WALK;
	}
	if (KEYMANAGER->isStayKeyDown('D'))
	{
		_x += _speed;
		_isLeft = false;
		_state = WALK;
	}
	if (KEYMANAGER->isStayKeyDown('W'))
	{
		
		_gravity -= 0.1f;
		_isJump = true;
		_state = JUMP;
	}

	if (KEYMANAGER->isStayKeyDown('S'))
	{
		if(_isJump == true)
		_y += _speed;
	}

	if (_isJump == true)
	{
		_y += -sinf(_angle) * _speed + _gravity ;
		_state = JUMP;
	}
	if (_gravity > 6 )
	{
		_y += _gravity;
		_state = JUMPFALL;
	}

	_gravity += 0.16f;
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);

	//위에 검사
	//for (int i = _hitBox.top; i < _hitBox.top + 1; i++)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.top);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_y = _hitBox.top + (OTUS_HEIGTH / 2);
			//break;
		}
	}
	//아래 검사
	//for (int i = _hitBox.bottom; i < _hitBox.bottom+1; i++)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.bottom);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// 마젠타가 아니면 검사
		{
			_y = _hitBox.bottom - (OTUS_HEIGTH / 2);
			_gravity = 0.f;
			_weight = 0.f;
			_isJump = false;
			//break;
			//IDLE상태 일려면?
			
		}
	}
	//왼쪽 검사
	//for (int i = _hitBox.left; i < _hitBox.left + 1; i++)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _hitBox.left, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// 마젠타가 아니면 검사
		{
			_x = _hitBox.left + (OTUS_WIDTH / 2 + _speed);
			//break;
		}
	}
	//오른쪽 검사
	//for (int i = _hitBox.right; i < _hitBox.right + 1; i++)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _hitBox.right, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// 마젠타가 아니면 검사
		{
			_x = _hitBox.right - (OTUS_WIDTH / 2 + _speed);
			//break;
		}
	}


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
}

void player::render()
{
	//cout << _gravity << endl;
	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
	}
	img[_state]->frameRender(getMemDC(), _x - CAM->getX() - img[_state]->getFrameWidth() / 2, _y - CAM->getY() - img[_state]->getFrameHeight() / 2);
	char str[128];
	sprintf_s(str, "X좌표 : %.3f Y좌표 : %.3f 중력 : %.3f", _x, _y,_gravity);
	TextOut(getMemDC(), 10, 10, str, strlen(str));
	//sprintf_s(str, "%d %d", _count, _index);
}

