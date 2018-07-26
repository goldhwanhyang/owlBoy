#include "stdafx.h"
#include "player.h"

HRESULT player::init()
{
	//�ε����� �̹��� ������
	img[IDLE] = IMAGEMANAGER->findImage("IDLE");
	img[WALK] = IMAGEMANAGER->findImage("Walk");
	img[JUMP] = IMAGEMANAGER->findImage("Jump");
	img[JUMPFALL] = IMAGEMANAGER->findImage("Fall");
	img[FLY] = IMAGEMANAGER->findImage("FLY");
	img[FLYDOWN] = IMAGEMANAGER->findImage("FLYDOWN");

	_isLeft = false;	// true = ���� , false = ������
	_isWalk = false;	// true = �ȴ�������
	_isJump = false;	// true = �����ϴ� ������
	_isFly = false;		// true = ����ٴϴ� ������
	_jumpCount = 0;

	_x = 440.f;			// �÷��̾� x��ǥ
	_y = 810.f;			// �÷��̾� y��ǥ
	_speed = 6.0f;		// �÷��̾� �ӵ�
	_angle = PI / 2;	// �÷��̾� ����
	_gravity = 0;		// �÷��̾� �߷�
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH); 
	_count = _index = 0;	// �������̹��� ������ ���� �ʱ�ȭ

	_state = IDLE;		// �⺻ ���� = �������ִ� ����

	return S_OK;
}

void player::release()
{

}

void player::update()
{
	if (_isJump == false && _isWalk == false) // ������ false, false�� �� IDLE�������������� �ٲ۴�.
	{
		_state = IDLE;
	}
	if (KEYMANAGER->isStayKeyDown('A'))	// AŰ�� ������ �������� ���ǵ常ŭ �̵��ϰ� �������� �ٴ��������� �����ش�.
	{
		_x -= _speed;
		_isLeft = true;
		_state = WALK;
	}
	if (KEYMANAGER->isStayKeyDown('D')) // DŰ�� ������ ���������� ���ǵ常ŭ �̵��ϰ� ���������� �ٴ� �������� �����ش�.
	{
		_x += _speed;
		_isLeft = false;
		_state = WALK;
	}
	if (KEYMANAGER->isStayKeyDown('W')) // WŰ�� ���� ������ �������� ���� �����ϰԲ� �߷°��� ���ְ� ���� �������� ������ ���� _isjump�� true�� �ٲ��ش�.
	{	
		_gravity -= 0.1f;
		_isJump = true;
	}
	if (KEYMANAGER->isOnceKeyDown('W'))
	{
		_jumpCount += 1;
		if (_jumpCount > 2)	// ����ī��Ʈ�� 2���� Ŀ���� 2�� ����
		{
			_jumpCount = 2;
		}
		if (_jumpCount == 2)	// ����ī��Ʈ�� 2�� �Ǹ� ����ٴϴ� �������� ������ _isFly�� true�� �ٲ��ش�.
		{
			_isFly = true;
		}
	}
	if (KEYMANAGER->isStayKeyDown('S')) 
	{
		_state = FLYDOWN;
		if (_isFly == true || _state == FLYDOWN)
		{
			_y += _speed;
		}
	}


	_gravity += 0.16f;		// �߷°��� 0.16�� �����ش�.
	if (_isJump == true)	// _isJump�� true�� ������ �ǰ� �����ϴ� �������� �����ش�.
	{
		_y += -sinf(_angle) * _speed + _gravity ;
		_state = JUMP;
	}
	if (_gravity > 6 )	// �߷°��� 6 �̻��� �Ǹ� ( ������ 0���� Ŀ�����ε� ����� �������ͼ� 6���� ���� ) �Ʒ��� �߷°��� �����ְ� �Ʒ��� �������� ����� �����ش�.
	{
		_y += _gravity;		
		_state = JUMPFALL;	
	}
	if (_isFly == true )
	{
		_isJump = false;
		_state = FLY;
		_gravity = 0;
	}
	// �Ʒ� �����¿� �˻��ϱ����� ��Ʈ�ڽ��� ���� ���� ���ְ� �˻縦�Ѵ�.
	// �������� ��Ʈ 
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);	

	//���� �˻�
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
	//�Ʒ� �˻�
	//for (int i = _hitBox.bottom; i < _hitBox.bottom+1; i++)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.bottom);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// ����Ÿ�� �ƴϸ� �˻�
		{
			_y = _hitBox.bottom - (OTUS_HEIGTH / 2);
			_gravity = 0.f;
			_weight = 0.f;
			_isJump = false;
			_jumpCount = 0;
			if (_state == FLY)
			{
				_isFly = false;
				_y = _hitBox.bottom - (OTUS_HEIGTH / 2 + _speed + 2);
			}
			//break;
		}
	}
	//���� �˻�
	//for (int i = _hitBox.left; i < _hitBox.left + 1; i++)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _hitBox.left, _y);
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
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _hitBox.right, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// ����Ÿ�� �ƴϸ� �˻�
		{
			_x = _hitBox.right - (OTUS_WIDTH / 2 + _speed);
			//break;
		}
	}

	//�ƿﺸ�� ������ ������
	_count++;
	if (_isLeft == false)		// ������
	{
		if (_state != JUMPFALL)			// �������� ���°� �ƴҶ� ��� �� if���� ������
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
		if (_state == JUMPFALL)	// �������� �����϶��� �� �������� ������
		{
			img[JUMPFALL]->setFrameY(0);
			if (_count % 7 == 0)
			{
				_index++;
				if (_index > img[JUMPFALL]->getMaxFrameX())
				{
					_index = img[JUMPFALL]->getMaxFrameX() - 2;
				}
				img[JUMPFALL]->setFrameX(_index);
			}
		}
	}
	if (_isLeft == true)			// ����
	{		
		if (_state != JUMPFALL)		// �������� ���°� �ƴ� �� ������ ������  ( �ٱ� , ���� ���)
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
		if (_state == JUMPFALL)	// �������� ������ ��
		{
			img[JUMPFALL]->setFrameY(1);
			if (_count % 7 == 0)
			{
				_index--;
				if (_index < 0)
				{
					_index = img[JUMPFALL]->getMaxFrameX()-2;
				}
				img[JUMPFALL]->setFrameX(_index);
			}
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
	sprintf_s(str, "X��ǥ : %.3f Y��ǥ : %.3f �߷� : %.3f ����ī��Ʈ : %d", _x, _y,_gravity, _jumpCount);
	TextOut(getMemDC(), 10, 10, str, strlen(str));
	//sprintf_s(str, "%d %d", _count, _index);
}

