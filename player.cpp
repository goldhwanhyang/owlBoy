#include "stdafx.h"
#include "player.h"

HRESULT player::init()
{
	//_testMap = IMAGEMANAGER->addImage("map", "Texture/player/1000x1000_map.bmp", 1000, 1000, true, RGB(255, 0, 255));
	_TownMap = IMAGEMANAGER->addImage("Town", "Texture/Maps/Town/townMap_5000x8000.bmp", 5000, 8000, true, RGB(255, 0, 255));
	_TownMapPixel = IMAGEMANAGER->addImage("TownPixel", "Texture/Maps/Town/townMapPixelCollision_5000x8000.bmp", 5000, 8000, true, RGB(255, 0, 255));
	CAM->setRange(_TownMap->getWidth(), _TownMap->getHeight());

	img[IDLE] = IMAGEMANAGER->addFrameImage("IDLE", "Texture/player/otusIdle_700x250_10x2.bmp", 700, 250, 10, 2);
	img[WALK] = IMAGEMANAGER->addFrameImage("Walk", "Texture/player/otusWalk_1000x270_8x2.bmp", 1000, 270, 8, 2);
	img[JUMP] = IMAGEMANAGER->addFrameImage("Jump", "Texture/player/otusJump_360x260_3x2.bmp", 360, 260, 3, 2);
	img[JUMPFALL] = IMAGEMANAGER->addFrameImage("Fall", "Texture/player/otusJumpFall_700x280_5x2.bmp", 700, 280, 5, 2);
	img[FLY] = IMAGEMANAGER->addFrameImage("FLY", "Texture/player/otusFly_1530x390_6x2.bmp", 1530, 390, 6, 2);

	_isLeft = false;	// ���� ������
	_isWalk = false;	
	_isJump = false;
	_isFly = false;	
	_x = 540.f;			// �÷��̾� x��ǥ
	_y = 465.f;			// �÷��̾� y��ǥ
	_speed = 6.0f;		// �÷��̾� �ӵ�
	_angle = PI / 2;
	_gravity = 0;		// �÷��̾� �߷�
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH); 
	_count = _index = 0;	// �������̹��� ������ ���� �ʱ�ȭ
	_weight = 0.f;
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

	//���� �˻�
	//for (int i = _hitBox.top; i < _hitBox.top + 1; i++)
	{
		COLORREF color = GetPixel(_TownMapPixel->getMemDC(), _x, _hitBox.top);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_y = _hitBox.top + (OTUS_HEIGTH / 2);
			//break;
		}
	}
	//�Ʒ� �˻�
	//for (int i = _hitBox.bottom; i < _hitBox.bottom+1; i++)
	{
		COLORREF color = GetPixel(_TownMapPixel->getMemDC(), _x, _hitBox.bottom);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// ����Ÿ�� �ƴϸ� �˻�
		{
			_y = _hitBox.bottom - (OTUS_HEIGTH / 2);


			_gravity = 0.f;
			_weight = 0.f;
			_isJump = false;
			//break;
			//IDLE���� �Ϸ��� �ٴڿ� ����־�� �ϰ� �������°� �ƴϰ� �޸��� ������ ��
			_state = IDLE;
		}
	}
	//���� �˻�
	//for (int i = _hitBox.left; i < _hitBox.left + 1; i++)
	{
		COLORREF color = GetPixel(_TownMapPixel->getMemDC(), _hitBox.left, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// ����Ÿ�� �ƴϸ� �˻�
		{
			_x = _hitBox.left + (OTUS_WIDTH / 2 + _speed);
			//break;
		}
	}
	//������ �˻�
	//for (int i = _hitBox.right; i < _hitBox.right + 1; i++)
	{
		COLORREF color = GetPixel(_TownMapPixel->getMemDC(), _hitBox.right, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// ����Ÿ�� �ƴϸ� �˻�
		{
			_x = _hitBox.right - (OTUS_WIDTH / 2 + _speed);
			//break;
		}
	}

	CAM->videoShooting(_x, _y);

	//�ƿﺸ�� ������ ������
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
	_TownMap->render(getMemDC(), CAM->getSX(), CAM->getSY(), CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY);
	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		_TownMapPixel->render(getMemDC(), CAM->getSX(), CAM->getSY(), CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY);
		Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
	}
	//cout << _gravity << endl;
	
	img[_state]->frameRender(getMemDC(), _x - CAM->getX() - img[_state]->getFrameWidth() / 2, _y - CAM->getY() - img[_state]->getFrameHeight() / 2);
	char str[128];
	sprintf_s(str, "X��ǥ : %.3f Y��ǥ : %.3f �߷� : %.3f", _x, _y,_gravity);
	TextOut(getMemDC(), 10, 10, str, strlen(str));
	//sprintf_s(str, "%d %d", _count, _index);
}

