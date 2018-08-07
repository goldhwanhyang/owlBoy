#include "stdafx.h"
#include "gawk.h"
#include "player.h"

HRESULT gawk::init(float x, float y, int dir)
{
	//CHECK �̹���
	//===============�̹��� �߰�============
	IGM->addFrameImage("��ũ_�⺻", "Texture/Enemies/Gawk/idle_1392x330_8x2.bmp", 1392, 330, 8, 2);
	IGM->addFrameImage("��ũ_����", "Texture/Enemies/Gawk/fly_1044x288_6x2.bmp", 1044, 288, 6, 2);
	IGM->addFrameImage("��ũ_����", "Texture/Enemies/Gawk/damaged_348x288_2x2.bmp", 348, 288, 2, 2);
	//=====================================
	enemy::init(x, y);

	_angle = 0.0f;
	_speed = 4.0f;
	_dir = dir;

	_gawkImage[READY] = IGM->findImage("��ũ_�⺻");
	_gawkImage[FLY] = IGM->findImage("��ũ_����");
	_gawkImage[STUN] = IGM->findImage("��ũ_����");
	_gawkImage[IDLE] = _gawkImage[READY];
	_gawkImage[FALL] = _gawkImage[FLY];

	_hitBox = RectMakeCenter(_x, _y, GAWK_CONST::HITBOX_WIDTH, GAWK_CONST::HITBOX_HEIGHT);
	_scanRc = RectMakeCenter(_x, _y, GAWK_CONST::HITBOX_WIDTH * 2, GAWK_CONST::HITBOX_HEIGHT * 14);

	_state = IDLE;
	_count = _index = 0;

	_delayCount = 0;
	_shakeAngle = 0;

	_hp = 3;
	_maxHp = 3;

	_power = 1;

	return S_OK;
}

void gawk::update()
{
	//TODO : �ӽ�
	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		damaged(_player);
	}

	_playerX = _player->getX();
	_playerY = _player->getY();

	_hitBox = RectMakeCenter(_x, _y, GAWK_CONST::HITBOX_WIDTH, GAWK_CONST::HITBOX_HEIGHT);
	_scanRc = RectMakeCenter(_x, _y, GAWK_CONST::HITBOX_WIDTH * 3, GAWK_CONST::HITBOX_HEIGHT * 14);
	//collide�� move�ڿ� �־�� ����� �۵��ߴ�.
	
	bool aniDone;
	if (_state == IDLE)
	{
		_index = 0;
	}
	else if (_state == FALL)
	{
		_index = _gawkImage[_state]->getMaxFrameX();
	}
	else
	{
		aniDone = frameMake(_gawkImage[_state]);
	}

	switch (_state)
	{
	case IDLE:
		search();
		turn();
		break;
	case READY:
		_y += 1.5;
		if (aniDone) _state = FLY;
		break;
	case FLY:
		move();
		turn();
		break;
	case FALL:
		move();
		turn();
		break;
	case STUN:
		stunShake();
		break;
	}

	collide();
}

void gawk::render()
{
	//��ġ����
	float tempX,tempY;
	if (_dir == RIGHT)
	{
		switch (_state)
		{
		case IDLE:
		case READY:
			tempX = _x - 95;
			tempY = _y - 90;
			break;
		case FLY:
		case FALL:
		case STUN:
			tempX = _x - 95;
			tempY = _y - 70;
			break;
		}		
	}
	else if (_dir == LEFT)
	{
		switch (_state)
		{
		case IDLE:
		case READY:
			tempX = _x - 70;
			tempY = _y - 90;
			break;
		case FLY:
		case FALL:
		case STUN:
			tempX = _x - 80;
			tempY = _y - 70;
			break;
		}
	}
	//����
	_gawkImage[_state]->frameRender(getMemDC(), tempX - CAM->getX(), tempY - CAM->getY(), _index, _dir);

	if (_isDebug)
	{
		char _debug[256];
		//Rectangle(getMemDC(), _scanRc.left - CAM->getX(), _scanRc.top - CAM->getY(), _scanRc.right - CAM->getX(), _scanRc.bottom - CAM->getY());
		Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
		//_stprintf_s(_debug, "����:%d, x:%f, y:%f ", _state, _x, _y );
		//TextOut(getMemDC(), _x, _y, _debug, strlen(_debug));
		//TextOut(getMemDC(), _x-CAM->getX(), _y-CAM->getY(), "X", strlen("X"));
		//_stprintf_s(_debug, "�߷�:%f, ������:%f", _gravity, _jumpDelay);
		//TextOut(getMemDC(), _x, _y+20, _debug, strlen(_debug));
	}
}

void gawk::release()
{
}
//���� ���� _shakeAngle�� oldState => count�� ����ϴ� shakeAngle�� ���������� oldState�� ���� �Ἥ ������ ������ �����.
//shakeAngle +1���� oldState�� ����,�����Ǿ� ����� ���� ������ �� ������
void gawk::damaged(actor* e)
{
	//POINT t = { _ptMouse.x + CAM->getX(), _ptMouse.y + CAM->getY() };
	//if (PtInRect(&_hitBox, t)) //TODO : �ӽ�
	{
		if(_state != STUN) _oldState = _state;
		_state = STUN;
		//TODO : �÷��̾������ŭ
		//_hp -= e->getPower();// _hp�� �Ű����� actor�� getPower��ŭ hp�� ��´�.
	}
	//CHECK ���ͽ��� ���ݰ� �Ե��� ������ �����ϴ� ��� -> �������� üũ
}


void gawk::stunShake()
{
	//Ŭ���ȵǴ� ������ update�� ������ ���������� ���� �������� �׷���.

	//===������ ������ ������====
	_shakeAngle += 0.25;
	_x += 5 * sinf(_shakeAngle); // sin���� x�� �����ϰ� �־ �������θ� ������
	//===========================
	if (_shakeAngle > PI*5)		//��Ŷ�� �ѹ��� ������
	{
		switch (_oldState)
		{
			case IDLE:
				_state = READY;
				break;
			case READY:
				_state = FLY;
				break;
			default:
				_state = _oldState;		//���� ���·� ������
			break;
		}
		_shakeAngle = 0;
	}
}

void gawk::move()
{
	if (_playerY - RND->getInt(40) > _y)
	{
		_jumpDelay = 800;
		_state = FALL;
	}
	else if (_playerY < _y)
	{
		_jumpDelay = 10;
		_state = FLY;
	}
	else
	{
		_jumpDelay = 30;
		_state = FLY;
	}
	//�߷°��ӵ�
	_gravity += RND->getFromFloatTo(0.02f, 0.05f);
	//������
	_delayCount = (_delayCount + 1) % _jumpDelay;
	if (_delayCount == 0)
	{
		_gravity = 0;
	}
	if (_dir == RIGHT)
	{
		_x += (_speed + RND->getFloat(1.5f)) * cosf(0);
	}
	else if (_dir == LEFT)
	{
		_x += (_speed + RND->getFloat(1.5f)) * cosf(PI);
	}
	_y += _speed * -sinf(1.57) + _gravity;

	//�÷��̾ ������ �� ��������
	//�÷��̾ ������ ����X, fly index�� getMaxFrameX�� ����
	//�÷��̾�� ���̰� ����ϸ� �����ϸ� �ٰ���
}

void gawk::collide()
{
	//����
	for (int i = _hitBox.top + _speed; i >= _hitBox.top; --i)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, i);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);
		if (!(r == 255 && g == 0 && b == 255))
		{
			_gravity = 6.f;
			_y = i + GAWK_CONST::HITBOX_HEIGHT / 2 + _gravity;
			break;
		}
	}
	//����
	for (int i = _hitBox.left + _speed; i >= _hitBox.left; --i)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), i, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);
		if (!(r == 255 && g == 0 && b == 255))
		{
			_dir = RIGHT;
			_x = i + GAWK_CONST::HITBOX_WIDTH / 2 +10;
			break;
		}
	}
	//������
	for (int i = _hitBox.right - _speed; i <= _hitBox.right; ++i)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), i, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_dir = LEFT;
			_x = i - GAWK_CONST::HITBOX_WIDTH / 2 - 10;
			break;
		}
	}
	//�Ʒ���
	for (int i = _hitBox.bottom - _speed; i <= _hitBox.bottom; i++)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, i);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			_gravity = 0;
			_y = i - GAWK_CONST::HITBOX_HEIGHT / 2 - 10;
			break;
		}
	}

	//�÷��̾�� �����ġ��
	RECT tempRc;
	if (IntersectRect(&tempRc, &_player->getHitbox(), &_hitBox))
	{
		//���� ) gawk�� �÷��̾�� ������� �ָ� �÷��̾�� �ڱ��ڽ��� hp�� ��´�.
		_player->damaged(this); //this�� �ڱ��ڽ��� ����Ű�� ������
	}
}

void gawk::search()
{
	POINT temp = { _playerX,_playerY };
	if (PtInRect(&_scanRc, temp))
	{
		_state = READY;
	}
}

void gawk::turn()
{
	if (_playerX - 200 < _x && _x < _playerX + 200)
	{
	}
	else if (_playerX + 200 >= _x)
	{
		_dir = RIGHT;
	}
	else
	{
		_dir = LEFT;
	}
}

bool gawk::frameMake(image * bmp)
{
	++_count;
	if (_count % RND->getFromIntTo(5,12) == 0)
	{
		++_index;
		if (_index > bmp->getMaxFrameX())
		{
			_index = 0;
			return true;
		}
	}

	return false;
}

