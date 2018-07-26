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
	img[ROLL] = IMAGEMANAGER->findImage("ROLL");

	_isLeft = false;	// true = ���� , false = ������
	_isWalk = false;	// true = �ȴ� ���
	_isJump = false;	// true = �����ϴ� ���
	_isFly = false;		// true = ����ٴϴ� ���
	_isFlyDown = false;	// true = ���߿��� �Ʒ��� �������� ���
	_jumpCount = 0;

	_x = 440.f;			// �÷��̾� x��ǥ
	_y = 810.f;			// �÷��̾� y��ǥ
	_speed = 6.0f;		// �÷��̾� �ӵ�
	_angle = PI / 2;	// �÷��̾� ����
	_gravity = 0;		// �÷��̾� �߷�
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH); 
	_count = _index = 0;	// �������̹��� ������ ���� �ʱ�ȭ

	_state = IDLE;		// �⺻ ���� = �������ִ� ����
	_beforeState = _state;

	return S_OK;
}

void player::release()
{

}

void player::update()
{
	if (_isJump == false && _isWalk == false && _state != ROLL) //�ȱ�,������ false�̰� ROLL���°� �ƴ� �� IDLE�������������� �ٲ۴�.
	{
		_state = IDLE;
	}
	this->inputKey();
	_gravity += 0.16f;		// �߷°��� 0.16�� �����ش�.
	if (_isJump == true)	// _isJump�� true�� ������ �ǰ� �����ϴ� �������� �����ش�.
	{
		_y += -sinf(_angle) * _speed + _gravity ;	
		_state = JUMP;
	}
	if (_gravity > 1.5f )	// �߷°��� 6 �̻��� �Ǹ� ( ������ 0���� Ŀ�����ε� ����� �������ͼ� 6���� ���� ) �Ʒ��� �߷°��� �����ְ� �Ʒ��� �������� ����� �����ش�.
	{
		_y += _gravity;		
		_state = JUMPFALL;	
		_jumpCount = 1;
	}
	if (_isFly == true )	// _isFly == true�� �� ������ �ȵǰ� false�� �ٲ��ְ� �����ִ� ����� �����ش�. �������� �ʰ� �߷°��� 0���� �ش�.
	{
		if (_state != ROLL)
		{
			_state = FLY;
		}	
		_isJump = false;
		_gravity = 0;
		// �����ִ� ���¿����� ������ _state�� FLY�� ROLL�� �ȵ�
		// _state�� FLY�� �ż� ROLL���°� �ȵ�~ �̰Ÿ� ������ ���·� �ٲ��ַ���?
		// _state �� ROLL�� �ٲ��ֱ� �����̽��ٸ� ������ ��������!
		// ��� ������ ��� �ұ� 
	}

	if (_isFlyDown == true)	// _isFlyDown == true�� �� �Ʒ��� �������� ����� �����ش�.
	{
		if(_state != ROLL)
		_state = FLYDOWN;
	}
	if (_state == ROLL)	// ���� �� ������ ó��
	{
		if (!_isLeft) // �������� ��
		{
			_x -= cosf(_angle * 2) * _speed * 2;
			//_x +=  _speed * 2 + 1;
			//if (_state == FLYDOWN)
			//{
			//	_y -= -sinf(_angle * 2) * _speed * 2;
			//}
		}
		if (_isLeft) // ������ ��
		{
			_x += cosf(_angle * 2) * _speed * 2;
			//_x -= _speed * 2 + 1;
		}
	}
	//fly���� �϶� �����̽��ٸ� ������ ������ �Ѵ�.
	
	// �Ʒ� �����¿� �˻��ϱ����� ��Ʈ�ڽ��� ���� ���� ���ְ� �˻縦�Ѵ�.
	// �������� ��Ʈ 
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);	

	//�浹ó���Լ�
	this->collide();
	//�ƿﺸ�� ������ ������
	this->frameSetting();
}

void player::render()
{
	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
	}
	img[_state]->frameRender(getMemDC(), _x - CAM->getX() - img[_state]->getFrameWidth() / 2, _y - CAM->getY() - img[_state]->getFrameHeight() / 2);
	char str[128];
	sprintf_s(str, "X��ǥ : %.3f Y��ǥ : %.3f �߷� : %.3f �ε��� : %d", _x, _y,_gravity, _index);
	TextOut(getMemDC(), 10, 10, str, strlen(str));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Ű�Է��Լ�
void player::inputKey()
{
	if (_state != ROLL)
	{
		if (KEYMANAGER->isStayKeyDown('A'))	// AŰ�� ������ �������� ���ǵ常ŭ �̵��ϰ� �������� �ٴ��������� �����ش�.
		{
			_x += cosf(_angle * 2) * _speed;
			//_x -= _speed;
			_isLeft = true;
			_state = WALK;
		}
		if (KEYMANAGER->isStayKeyDown('D')) // DŰ�� ������ ���������� ���ǵ常ŭ �̵��ϰ� ���������� �ٴ� �������� �����ش�.
		{
			_x -= cosf(_angle * 2) * _speed ;
			//_x += _speed;
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
		if (KEYMANAGER->isStayKeyDown('S') && _isFly == true)
		{
			// IDLE�϶��� �������� , �������¿��� FLYDOWN�� �ǰ� ,

			_isFlyDown = true;		// �Ʒ��� ������ �� true�� �ǰ�
			if (_isFly == true || _state == FLYDOWN )	// ���� �ְų� ���°� �Ʒ��� �������� ������ ��
			{
				_y += _speed;	// _y�� ���ǵ常ŭ �����ش�. (�Ʒ��� ������ )
			}
		}
		if (KEYMANAGER->isOnceKeyUp('S'))	// SŰ���� ���� ���ȴ� ������ _isFlyDown = false;
		{
			_isFlyDown = false;
		}
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE) && _state != ROLL)
		{
			_count = 0;
			_state = ROLL;
			_index = 0;	
		}
	}
}


//�浹�Լ�
void player::collide()
{
	//���� �˻�
	//for (int i = _hitBox.top; i < _hitBox.top + 1; i++)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.top);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		//isFly�϶� isFlyDown�� �ƴ� �� �浹 �˻�
		if ((r == 0 && g == 0 && b == 0)) // �������� �˻�
		{
			_y = _hitBox.top + (OTUS_HEIGTH / 2 + _speed);
			//break;
		}
	}
	//�Ʒ� �˻�
	//for (int i = _hitBox.bottom-10; i < _hitBox.bottom; i++)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.bottom);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// ����Ÿ�� �ƴϸ� �˻�
		{
			_y = _hitBox.bottom - (OTUS_HEIGTH / 2);
			_gravity = 0.f;
			_isJump = false;
			_jumpCount = 0;

			if (_state == FLYDOWN)
			{
				_isFlyDown = false;
				_isFly = false;
				_state = IDLE;
			}
			//���߿��� �����ִ� ������ ��
			if (_state == FLY)
			{
				//_y = _hitBox.bottom - (OTUS_HEIGTH / 2+ _speed);
				_y += _speed;
				_isFly = false;
				_isFlyDown = false;
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

		if ((r == 0 && g == 0 && b == 0)) // ����Ÿ�� �ƴϸ� �˻� ���ٰ� �������̸� �˻�
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

		if ((r == 0 && g == 0 && b == 0))	// ����Ÿ�� �ƴϸ� �˻� ����Ÿ�� ����
											//�������� ���������� �ʷϻ��� ����ġ�� �ҷ��� 
		{
			_x = _hitBox.right - (OTUS_WIDTH / 2 + _speed);
			//break;
		}
	}

}


//�̹��������Ӽ���
void player::frameSetting()
{
	_count++;
	// ������ �ΰ�������?�� ������ ������ ���� ������ �ٲ��� �ʾƼ� �ӽ÷� ������ ������
	if (_state == ROLL)		// ������ ������ ��
	{
		if(_isLeft)
			img[ROLL]->setFrameY(1);
		else
			img[ROLL]->setFrameY(0);
		if (_count % 5 == 0)	// ī��Ʈ�� 5�� ������ 0�� �ɶ�����
		{
			_index++;			// �ε����� 1�� ����
			if (_index > img[_state]->getMaxFrameX())		// �ε����� 0���� �۾������� �ϸ�
			{
				_state = _beforeState;	// �������·�
				_index = 0;
			}
			img[_state]->setFrameX(_index);

		}
		return;
	}

	if (_isLeft == false)		// ������
	{
		if (_state != JUMPFALL && _state != ROLL)			// �������� ���°� �ƴҶ� ��� �� if���� ������
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
		if (_state == JUMPFALL && _state != ROLL)	// �������� �����϶��� �� �������� ������.
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
		//if (_state == ROLL)			// ���°� ������ ���� �� ��
		//{
		//	img[ROLL]->setFrameY(0); 
		//	if (_count % 5 == 0)
		//	{
		//		_index++;
		//		if (_index > img[ROLL]->getMaxFrameX())	// �ƽ�������X���� Ŀ����
		//		{
		//			_state = _beforeState;	// ���� = ��������
		//			//_isRoll = false;
		//			//_state = IDLE;
		//		}
		//		img[ROLL]->setFrameX(_index);
		//	}
		//}
	}
	if (_isLeft == true)			// ����
	{
		if (_state != JUMPFALL && _state != ROLL)		// �������� ���°� �ƴ� ��, ������ ���°� �ƴ� �� ������ �������� ������ ( �ٱ� , ���� ���)
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
		if (_state == JUMPFALL && _state != ROLL)	// �������� ������ ��
		{
			img[JUMPFALL]->setFrameY(1);
			if (_count % 7 == 0)
			{
				_index--;
				if (_index < 0)
				{
					_index = img[JUMPFALL]->getMaxFrameX() - 2;
				}
				img[JUMPFALL]->setFrameX(_index);
			}
		}
		//if (_state == ROLL)		// ������ ������ ��
		//{
		//	img[ROLL]->setFrameY(1);
		//	if (_count % 5 == 0)	// ī��Ʈ�� 5�� ������ 0�� �ɶ�����
		//	{
		//		_index--;			// �ε����� 1�� ����
		//		if (_index < 0)		// �ε����� 0���� �۾������� �ϸ�
		//		{
		//			_state = _beforeState;	// �������·�
		//		}
		//		img[ROLL]->setFrameX(_index);
		//	}
		//}
	}
}
