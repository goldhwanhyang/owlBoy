#include "stdafx.h"
#include "player.h"

HRESULT player::init()
{
	_testMap = IMAGEMANAGER->addImage("map", "Texture/player/1000x1000_map.bmp", 1000, 1000, true, RGB(255, 0, 255));
	CAM->setRange(_testMap->getWidth(), _testMap->getHeight());

	//img = IMAGEMANAGER->addFrameImage("owl", "Texture/player/fly_7x2.bmp", 2373, 580, 7, 2, true, RGB(255, 0, 255));
	img = IMAGEMANAGER->addFrameImage("owl", "Texture/player/fly_949x232_7x2.bmp", 949, 232, 7, 2, true, RGB(255, 0, 255));
	_x = 140.f;	// �÷��̾� x��ǥ
	_y = 450.f; // �÷��̾� y��ǥ
	_speed = 5.0f;	// �÷��̾� �ӵ�
	_proveY = _y + img->getHeight()/2; 
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

	//�ƿﺸ�� �ȼ��浹 �ϴ� �κ�
		// ž
	//for (int i = _hitBox.top; i < _hitBox.top-10; i++)	// ó�� ���� i ���� �ִ���� i++�ϸ鼭 �˻� 
	//{
	//	COLORREF color = GetPixel(_testMap->getMemDC(), _x, i);
	//	int r = GetRValue(color);
	//	int g = GetGValue(color);
	//	int b = GetBValue(color);
	//
	//	if (!(r == 255 && g == 0 && b == 255))	// ����Ÿ�� �ƴϸ� �˻�
	//	{
	//		if (isDrop == false)
	//		{
	//			_y = _hitBox.top;
	//		} 
	//		break;
	//	}
	//}
	for (int i = _hitBox.bottom; i < _hitBox.bottom + 5; i++)
	{
		COLORREF color = GetPixel(_testMap->getMemDC(), _x, i);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// ����Ÿ�� �ƴϸ� �˻�
		{
			//_y = i - _hitBox.bottom+350;		
			_y = _hitBox.bottom-30;
			break;
		}
	}

	// �ƿﺸ�� ��Ʈ
	// �������� = �÷��̾��� y��ǥ + �̹�������ũ��/2 ��ŭ
}

void player::render()
{
	_testMap->render(getMemDC());

	RECT rc = RectMakeCenter(_x, _hitBox.bottom, 10, 10);	// �������� ��Ʈ
	Rectangle(getMemDC(), rc);
	Rectangle(getMemDC(), _hitBox);
	
	img->frameRender(getMemDC(), _x-img->getFrameWidth()/2, _y-img->getFrameHeight()/2);

	char str[128];
	sprintf_s(str, "%.3f %.3f", _x, _y);
	TextOut(getMemDC(), 10, 10, str, strlen(str));
	//sprintf_s(str, "%d %d", _count, _index);
}

