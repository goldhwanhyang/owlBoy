#include "stdafx.h"
#include "geddy.h"
#include "player.h"


HRESULT geddy::init()
{
	liftableActor::init();
	_img[IDLE] = IMAGEMANAGER->findImage("GEDDY_IDLE");
	_img[ON_AIR] = IMAGEMANAGER->findImage("GEDDY_ON_AIR");
	_img[ATTACK] = IMAGEMANAGER->findImage("GEDDY_SHOOT");
	_img[HANG] = _img[ATTACK];
	_img[WALK] = IMAGEMANAGER->findImage("GEDDY_WALK");
	_img[PREFARE] = IMAGEMANAGER->findImage("GEDDY_PREPARE_SHOOT");
	_handImg = IMAGEMANAGER->findImage("GEDDY_HAND");

	_state = IDLE;

	_bullet = new bullet[MAX_GEDDY_BULLET];
	for (int i = 0; i < MAX_GEDDY_BULLET; ++i)
	{
		_bullet[i].init(5, 10, 500);
	}

	_x = 440.f;			// �÷��̾� x��ǥ
	_y = 810.f;			// �÷��̾� y��ǥ
	_speed = 6.0f;
	_angle = 0;	// �÷��̾� ����
	_gravity = 0;		// �÷��̾� �߷�
	_count = _index = 0;	// �������̹��� ������ ���� �ʱ�ȭ
	_curFrameX = _curFrameY = 0;

	_handsDir = _shootingDir = 0;

	_maxWidth = 100;
	_maxHeight = 100;
	return S_OK;
}

void geddy::release()
{
	for (int i = 0; i < MAX_GEDDY_BULLET; ++i)
	{
		_bullet[i].release();
		SAFE_DELETE_ARRAY(_bullet);
	}
}

void geddy::update()
{
	_hitBox = RectMakeCenter(_x, _y, GEDDY_WIDTH, GEDDY_HEIGHT);

	if (_state == HANG)
	{
		_z = 15;
		_angle = getAnglef(_x - CAM->getX(), _y - CAM->getY(), _ptMouse.x, _ptMouse.y);
		convertDir();

		CAM->videoShooting(_x, 
			_y,
			0);
	}
	else
	{
		_z = 0;
		if (_angle > PI / 2 && _angle < 3 * PI / 2)
			_dir = 1;
		else
			_dir = 0;
	}

	for (int i = 0; i < MAX_GEDDY_BULLET; ++i)
	{
		if (!_bullet[i].getIsActive()) continue;
		_bullet[i].update();
		if (_bullet[i].collide(_mapPixel))
		{
			float angle = _bullet[i].getEffectAngle() - PI / 2;
			if (angle < 0)
			{
				angle += 2 * PI;
			}
			EFFECTMANAGER->play("�Ѿ�����", _bullet[i].getX(), _bullet[i].getY(), angle);
		}
	}
}

void geddy::render()
{
	for (int i = 0; i < MAX_GEDDY_BULLET; ++i)
	{
		_bullet[i].render();
	}


	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
	}

	_img[_state]->frameRender(getMemDC(),
		_x - _img[_state]->getFrameWidth()/2 - CAM->getX(),
		_y - _img[_state]->getFrameHeight()/2 - CAM->getY(),
		_curFrameX, _dir);

	if (_state == HANG)
	{
		float angle = _angle;
		int temp;
		if (PI / 2 < angle && angle < 3 * PI / 2)
		{
			angle = angle - PI;
			temp = 15;
		}
		else
		{
			temp = -15;
		}
		_handImg->rotateFrameRender(getMemDC(),
			_x - CAM->getX() + temp,
			_y - CAM->getY(),
			0, _handsDir, angle);
	}
	else if (_state != IDLE)
	{
		move();
		collide();
	}
}

int geddy::convertDir()
{
	if (30 * PI / 180 <= _angle && _angle < PI / 2)	// 
	{
		_dir = 1;
		_handsDir = 1;
		
	}
	else if (PI / 2 <= _angle && _angle < 150 * PI / 180)	//
	{
		_dir = 4;
		_handsDir = 3;
	}
	else if (150 * PI / 180 <= _angle && _angle < 210 * PI / 180)	// 
	{
		_dir = 3;
		_handsDir = 2;
	}
	else if (210 * PI / 180 <= _angle && _angle < 3 * PI / 2)	//
	{
		_dir = 5;
		_handsDir = 3;
	}
	else if (3 * PI / 2 <= _angle && _angle < 330 * PI / 180)	// 
	{
		_dir = 2;
		_handsDir = 1;
	}
	else
	{
		_dir = 0;
		_handsDir = 0;
	}

	return 0;
	
}


void geddy::attack()
{
	for (int i = 0; i < MAX_GEDDY_BULLET; ++i)
	{
		if (_bullet[i].getIsActive()) continue;

		_bullet[i].setFireCenter(_x, _y);
		_bullet[i].setAngle(_angle * 180 / PI);
		_bullet[i].setIsActive(true);
		break;
	}

}

void geddy::damaged(actor * e)
{
}

void geddy::move()
{
	liftableActor::move();
}

void geddy::collide()
{

	COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.top);
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);

	if ((r == 0 && g == 0 && b == 0)) // �������� �˻�
	{
		_y = _hitBox.top + (_hitBox.bottom - _hitBox.top) / 2;
		_angle = 3 * PI / 2;
		//break;
	}

	//�Ʒ� �˻�
	color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.bottom);
	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);

	if (!(r == 255 && g == 0 && b == 255) && _state != HANG)	// ����Ÿ�� �ƴϸ� �˻�
	{
		_y = _hitBox.bottom - (_hitBox.bottom - _hitBox.top) / 2;
		_state = IDLE;
	}

	//���� �˻�
	color = GetPixel(_mapPixel->getMemDC(), _hitBox.left, _y);
	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);

	if ((r == 0 && g == 0 && b == 0)) // ����Ÿ�� �ƴϸ� �˻� ���ٰ� �������̸� �˻�
	{
		_x = _hitBox.left + (_hitBox.right - _hitBox.left) / 2;
		_angle = 3 * PI / 2;
		//break;
	}

	//������ �˻�
	color = GetPixel(_mapPixel->getMemDC(), _hitBox.right, _y);
	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);

	if ((r == 0 && g == 0 && b == 0))	// ����Ÿ�� �ƴϸ� �˻� ����Ÿ�� ����
										//�������� ���������� �ʷϻ��� ����ġ�� �ҷ��� 
	{
		_x = _hitBox.right - (_hitBox.right - _hitBox.left) / 2;
		_angle = 3 * PI / 2;
		//break;
	}
}

void geddy::lifted(player * _player)
{
	_state = HANG;
	_x = _player->getX();
	_y = _player->getY() + _maxHeight / 2;
}

void geddy::drawUI()
{
}
