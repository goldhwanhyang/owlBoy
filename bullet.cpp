#include "stdafx.h"
#include "bullet.h"
HRESULT bullet::init(float radius, float speed, float range, const char * imageName)
{
	if (strcmp(imageName, "없음") != 0) //문자열 비교함수 strcmp 
		_image = IMAGEMANAGER->findImage(imageName);
	else
		_image = NULL;
	_radius = radius;
	_speed = speed;
	_range = range;
	_isActive = false;

	_index = 0;

	return S_OK;
}

void bullet::update()
{
	if (_isActive)
	{
		_x += _speed * cosf(_angle * 0.017);
		_y += _speed * -sinf(_angle * 0.017);
		_hitBox = RectMakeCenter(_x-CAM->getX(), _y-CAM->getY(), _radius * 2, _radius * 2);

		if (_image != NULL)
		{
			++_count;
			if (_count % 7 == 0) ++_index;
			if (_index > _image->getMaxFrameX()) _index = 0;
		}
	}
	collide();
}

void bullet::render()
{
	if (_isActive)
	{
		if (_image == NULL) Ellipse(getMemDC(), _hitBox);
		else _image->frameRender(getMemDC(), _x-CAM->getX(), _y-CAM->getY(), _index, _dir); //회전렌더로 바꾸자
	}

	if (_isDebug)
	{
		RectangleLine(getMemDC(), _hitBox);
		SetBkMode(getMemDC(), OPAQUE);
		wsprintf(_debug, "%d", _index);
		TextOut(getMemDC(), _x, _y, _debug, strlen(_debug));
	}
}

void bullet::release()
{

}

void bullet::collide()
{
	//벽과 충돌할떄
	COLORREF color = GetPixel(IMAGEMANAGER->findImage("보스방1픽셀")->getMemDC(), _x, _y);
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);
	if (r == 0 && g == 0 && b == 0)
	{
		if(_isActive) _isActive = false;
	}

	//사거리 벗어남
	float distance = getDistance(_fireX, _fireY, _x, _y);
	if (_range < distance)
	{
		_isActive = false;
	}

	//플레이어랑 충돌도 만들자
}
