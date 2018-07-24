#include "stdafx.h"
#include "player.h"

HRESULT player::init()
{
	_testMap = IMAGEMANAGER->addImage("map", "Texture/player/1000x1000_map.bmp", 1000, 1000, true, RGB(255, 0, 255));
	CAM->setRange(_testMap->getWidth(), _testMap->getHeight());

	//img = IMAGEMANAGER->addFrameImage("owl", "Texture/player/fly_7x2.bmp", 2373, 580, 7, 2, true, RGB(255, 0, 255));
	img = IMAGEMANAGER->addFrameImage("owl", "Texture/player/otusFly_949x232_7x2.bmp", 949, 232, 7, 2, true, RGB(255, 0, 255));
	_x = 140.f;	// �÷��̾� x��ǥ
	_y = 450.f; // �÷��̾� y��ǥ
	_speed = 5.0f;	// �÷��̾� �ӵ�
	//_proveY = _y + img->getHeight()/2; 
	// �ƿﺸ���� y�� �������� ( �ƿﺸ�� �Ʒ��� ���� �簢�� ����� )
	// �÷��̾��� y��ǥ + �̹�������ũ�� / 2 ��ŭ ����
	_hitBox = RectMakeCenter(_x, _y, img->getFrameWidth(), img->getFrameHeight()); // 
	_count = _index = 0;	// �������̹��� ������ ���� �ʱ�ȭ

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
	}
	if (KEYMANAGER->isStayKeyDown('D'))
	{
		_x += _speed;
	}
	if (KEYMANAGER->isStayKeyDown('W'))
	{
		_y -= _speed;
	}
	if (KEYMANAGER->isStayKeyDown('S'))
	{
		_y += _speed;
	}
	//�ƿﺸ�� ������ ������
	_count++;
	img->setFrameY(0);
	if (_count % 7 == 0)
	{
		_index++;
		if (_index > img->getMaxFrameX())
		{
			_index = 0;
		}
		img->setFrameX(_index);
	}
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);

	//���� �˻�
	for (int i = _hitBox.top; i < _hitBox.top + 1; i++)
	{
		COLORREF color = GetPixel(_testMap->getMemDC(), _x, i);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_y = _hitBox.top + 30;
			break;
		}
	}
	//�Ʒ� �˻�
	for (int i = _hitBox.bottom; i < _hitBox.bottom+1; i++)
	{
		COLORREF color = GetPixel(_testMap->getMemDC(), _x, i);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);
	
		if (!(r == 255 && g == 0 && b == 255))	// ����Ÿ�� �ƴϸ� �˻�
		{
			_y = _hitBox.bottom-30;
			break;
		}
	}
	//���� �˻�
	for (int i = _hitBox.left; i < _hitBox.left + 1; i++)
	{
		COLORREF color = GetPixel(_testMap->getMemDC(), i, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// ����Ÿ�� �ƴϸ� �˻�
		{
			_x = _hitBox.left + 20;
			break;
		}
	}
	//������ �˻�
	for (int i = _hitBox.right; i < _hitBox.right + 1; i++)
	{
		COLORREF color = GetPixel(_testMap->getMemDC(), i, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// ����Ÿ�� �ƴϸ� �˻�
		{
			_x = _hitBox.right - 20;
			break;
		}
	}
}

void player::render()
{
	_testMap->render(getMemDC());
	RECT topRc = RectMakeCenter(_x, _hitBox.top, 10, 10);
	//Rectangle(getMemDC(), topRc);
	RECT BottomRc = RectMakeCenter(_x, _hitBox.bottom, 10, 10);	// ���� 
	//Rectangle(getMemDC(), BottomRc);
	Rectangle(getMemDC(), _hitBox);
	
	img->frameRender(getMemDC(), _x-img->getFrameWidth()/2, _y-img->getFrameHeight()/2);

	char str[128];
	sprintf_s(str, "%.3f %.3f", _x, _y);
	TextOut(getMemDC(), 10, 10, str, strlen(str));
	//sprintf_s(str, "%d %d", _count, _index);
}

