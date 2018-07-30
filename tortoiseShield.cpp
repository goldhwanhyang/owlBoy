#include "stdafx.h"
#include "tortoiseShield.h"

HRESULT tortoiseShield::init(float x, float y)
{
	enemy::init(x, y);

	_image = IGM->findImage("거북이_떨어진방패");
	_isActive = true;
	_speed = 2.8f;
	_gravity = 0;
	return S_OK;
}

void tortoiseShield::update()
{

	if (!_isActive) 
	{
		move();
		collide();
		_hitBox = RectMakeCenter(_x, _y, 220, 160);
		//방패 프레임 돌리기, 맥스프레임이면 프레임 안돈다.
		if (_index != _image->getMaxFrameX())
		{
			_count++;
			if (_count % 7 == 0)
			{
				_index++;
			}
		}
	}

	if (_isActive)
	{
		_hitBox = RectMakeCenter(_x, _y + 20, 220, 240);
		_index = 0; //실드가 달려있을땐 인덱스를 0으로
		_gravity = 0;
	}
}

void tortoiseShield::render()
{
	if (!_isActive)
	{
		_image->frameRender(getMemDC(), _x - 95 - CAM->getX(), _y - 50 - CAM->getY(), _index, _dir);
	}
	if (_isDebug)
	{
		_stprintf_s(_debug, "angle: %f, speed: %f", _angle, _speed);
		TextOut(getMemDC(), 100, 120, _debug, strlen(_debug));
		//Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
	}
}

void tortoiseShield::release()
{
}

void tortoiseShield::damaged(actor * e)
{
	//떨어진 실드를 회전공격으로 때렸을때의 반응
}

void tortoiseShield::move()
{
	_gravity += 0.05f;
	if (310 < _x && _x < IMAGEMANAGER->findImage("보스방1")->getWidth() - 310)
	{
		_x += _speed * cos(PI - _angle);
	}
	_y += _speed * -sin(_angle) + _gravity;
}

void tortoiseShield::collide()
{
	/* //TODO : 추후에 플레이어가 들고 나를것까지 생각해야한다.

	COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.top);
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);

	if ((r == 0 && g == 0 && b == 0)) // 검은색만 검사
	{
		_y = _hitBox.top + (_hitBox.bottom - _hitBox.top) / 2;
		_angle = 3 * PI / 2;
		//break;
	}

	//아래 검사
	color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.bottom);
	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);

	if (!(r == 255 && g == 0 && b == 255))	// 마젠타가 아니면 검사
	{
		_y = _hitBox.bottom - (_hitBox.bottom - _hitBox.top) / 2;
	}

	//왼쪽 검사
	color = GetPixel(_mapPixel->getMemDC(), _hitBox.left, _y);
	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);

	if ((r == 0 && g == 0 && b == 0)) // 마젠타가 아니면 검사 였다가 검은색이면 검사
	{
		_x = _hitBox.left + (_hitBox.right - _hitBox.left) / 2;
		_angle = 3 * PI / 2;
		//break;
	}

	//오른쪽 검사
	color = GetPixel(_mapPixel->getMemDC(), _hitBox.right, _y);
	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);

	if ((r == 0 && g == 0 && b == 0))	// 마젠타가 아니면 검사 마젠타를 무시
										//검은색은 안지나가고 초록색은 지나치게 할려면 
	{
		_x = _hitBox.right - (_hitBox.right - _hitBox.left) / 2;
		_angle = 3 * PI / 2;
		//break;
	}
	*/

	//픽셀충돌
	COLORREF color = GetPixel(IMAGEMANAGER->findImage("보스방1픽셀")->getMemDC(), _x, _hitBox.bottom);
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);

	if (!(r == 255 && g == 0 && b == 255))
	{
		_y = _hitBox.bottom - 90;
		_speed = 0;
		_gravity = 0;
	}
}