#include "stdafx.h"
#include "player.h"

/*
	�ڵ��� �� ���� ���� �� ����
	������ ��� �ɾ����� �� ������ ����
	�ɾ��� ������ �߸����� �ʾҴ��� �� ���캸��
*/


//FLYDOWN�� JUMPFALL�ߴ���ó�� ������ġ ������ġ Ȯ���ؼ� �Ʒ��� ���������� FLYDOWN�ǰ�
//�������� �����ε� ������� ����ٴϴ� ����
//FLY�߿� ���ΰ��� �ٴ� ��������

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
	img[ATK] = IMAGEMANAGER->findImage("ATTACK");

	_isLeft = false;	// true = ���� , false = ������

	_x = 440.f;					// �÷��̾� x��ǥ
	_y = 810.f;					// �÷��̾� y��ǥ
	_speed = 7.0f;				// �÷��̾� �޸��� �ӵ� ( ���߿� ���¿� ���� �������� �� �ӵ�,���������� �ӵ�, ������ �ӵ� ��� ���¿� ���� �ӵ� �ٲ��ֱ� )
	_jumpSpeed = 15.f;			// �÷��̾� ���� �ӵ�
	_jumpCount = 0;
	_flySpeed = 10.f;
	_angle = PI / 2;			// �÷��̾� ����
	_gravity = 0;				// �÷��̾� �߷�
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH); 
	_count = _index = 0;		// �������̹��� ������ ���� �ʱ�ȭ

	_state = IDLE;				// �⺻ ���� = �������ִ� ���� (�÷��̾����)
	_axisX = _axisY = NONE;		// Ű�� �ȴ��� ���� (Ű ����)
	_FX = _FY = FLY_N;			// �������� �� Ű ���� �ʱ�ȭ NONE
	_beforeState = _state;
	_oldY = 0;					// ���� ��ġ

	return S_OK;
}


void player::release()
{


}


void player::update()
{
	// ���� ���� ��
	//if (_state == FLY || _state == FLYDOWN)	// �����ְų� �Ʒ��� �������� ������ ��
	//if(_jumpCount == 2)		// jumpCount 2�� �Ǹ� �����ִ� ���·� ����
	if(_state == FLY || _state == FLYDOWN)
	{
		_speed = 10.f;	
		_gravity = 0;
		flyInputKey();
		flyAngle(_FX, _FY);
		flyMove();
	}
	// ���� ���� ��
	else
	{
		_speed = 7.0f;													// ���� ���� �� ���ǵ� 7.0f
		groundInputKey();												// ���� ���� �� �Է��ϴ� Ű
		groundAxis(_axisX, _axisY);
		//groundMove();
	}
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
	//img[_state]->setFrameY(_dir);
	img[_state]->frameRender(getMemDC(), _x - CAM->getX() - img[_state]->getFrameWidth() / 2, _y - CAM->getY() - img[_state]->getFrameHeight() / 2);
	char str[128];
	sprintf_s(str, "X��ǥ : %.f  Y��ǥ : %.f  �߷� : %.f �ε��� : %d  ���ǵ� : %.f   ���� : %.2f   x�� : %d   y�� : %d ���� : %d  ����ī��Ʈ : %d", _x, _y,_gravity, _index, _speed, _angle, _axisX, _axisY,_state, _jumpCount);
	TextOut(getMemDC(), 10, 10, str, strlen(str));
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Ű�Է��Լ� ( �Ұ�, ���������� �ٲ��ֱ� )
void player::groundInputKey()
{
	_axisX = NONE;										// Ű �Է��� ������ _axisX �� NONE�� �ȴ�.
	if (_state == JUMPFALL)								// �������� ���� �� W�� ������ �ٷ� �����ִ� ���°� �� �� �ְ� _jumpCount�� 1�� ���ְ�
		_jumpCount = 1;
	if (KEYMANAGER->isStayKeyDown('A'))		
	{
		_dir = LEFT;
		_isLeft = true;									// ������ ���� ���� true�� ���� �������� �����ش�.
		_axisX = LEFT;									// LEFT �������� �̵�
		if (_state == ATK)
			changeState(ATK);
		else if (_state != JUMP && _state != JUMPFALL)		// ������ �������� ������ �� �ٴ¸�� �ȳ����� ���ǿ� �ɾ��ش�.
			changeState(WALK);							// ������Ʈ�� �ٲ� ���� index, count 0�����ʱ�ȭ���ִ� �Լ�.
	}
	else if (KEYMANAGER->isStayKeyDown('D'))
	{
		_dir = RIGHT;
		_isLeft = false;								// ������ ���� ���� false�� ������ �������� �����ش�.
		_axisX = RIGHT;									// RIGHT�� ���������� �̵�
		if (_state == ATK)
			changeState(ATK);
		else if (_state != JUMP && _state != JUMPFALL)		// ������ �������� ������ �� �ٴ¸���� �ȳ����� ������ �ɾ��ش�.
			changeState(WALK);							// ������Ʈ�� �ٲ� ���� index, count 0�����ʱ�ȭ���ִ� �Լ�.
		
	}
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON) && _state != ATK)
	{
		changeState(ATK); // �ڵ带 ���ٷ� ������ �Լ��� ����µ� �޶����� ������ �Ű������� ���ش�.
		
	}
	if (_state != JUMP )	// Ű�� �ȴ����� �ٷ� NONE�� �ż� ������ �ȵ�
		_axisY = NONE;
	if (KEYMANAGER->isOnceKeyDown('W'))
	{
		_axisY = UP;									// UP�� �Ǹ� ���� ������ �Ѵ�.
		changeState(JUMP);								// ���� �����ϴ� �������� �����ش�.
		_jumpCount += 1;								// _jumpCount�� 2�� �Ǹ� FLY���·� �ٲ��ش�.
		if (_jumpCount > 1)
		{
			_jumpCount = 2;
			_state = FLY;
			_index = 0;
			_count = 0;
		}
	}
	else if (KEYMANAGER->isStayKeyDown('W'))		
	{
		_gravity -= 0.16f;								// WŰ�� ���� ������ ���� ���� ���� ������ �� �ִ�.
	}
	//_click = NB;	// 
}

// ���� �� �κ�
void player::groundAxis(WAY axisX, WAY axisY)	// Ű �Է����� �ٲ��� ���³� �Ұ��� ���� �������ֱ�
{
	_oldY = _y;									// ���� ��ġ(_oldY)�� ������ġ(_y)�� �����Ѵ�.
	if (_axisX == NONE && _axisY == NONE && _state != ATK)		// X,Y Ű�� �� �� NONE�̸� _state = IDLE �����̴�.
		_state = IDLE;

	/*
	_y�� 0�� �������ϱ�. ������ �߿��ϴ�.
	_gravity += 0.5f�� ���� ������ ���� ��Ƶ� 0�̾ƴ϶� 0.5f�� �������� JUMPFALL���°� �ż� �Ʒ��� ��������.
	_y�� ���� ������ ��� 0�� ��������.
	*/
	_y += _gravity;		// y�� �߷°��� ��� �����ش�. 
	_gravity += 0.5f;	// �Ʒ��� ������ �� �ְ� ������ �ƴϾ ��� �߷°��� �����ش�.

	if (_axisY == UP)	// _axisY == UP�� �� ���� ������ �����ش�.
	{
		_y += -sinf(PI / 2) * _jumpSpeed;
	}

	/* 
	-sinf(angle) * speed << ������
	_gravity += -sinf(angle) * speed; << �̰Ÿ� ��� ���ϸ� �Ʒ��� �������°� �ƴ϶� ������ ������ ��
	�׷��� �ѹ��� �����ϰ� ������ �ɾ���� �ϴµ� . 

	*/

	//�������� ��Ȳ : �߷��� �ӵ����� Ŀ����, ������ġ�� ������ġ���� Ŭ��
	//�ݵ�� �����ϱ� �߿� 
	if (_y > _oldY)
	{
		// �ѹ��� �����������Ѵ�.
		// ������ �ߴٰ� �������� �ڿ������� �������� ���� �߷��� ����
		if(_state == JUMP)
			_gravity += -sinf(PI / 2) * _jumpSpeed;	

		changeState(JUMPFALL);			// ������Ʈ�� �ٲ� ���� index, count 0�����ʱ�ȭ���ִ� �Լ�.
	}

	if (_axisX == LEFT)		//  _axisX == LEFT�� �� �������� �������ش�.
	{
		_x -= _speed;
	}
	else if (_axisX == RIGHT)	//_axisX == RIGHT�� �� ���������� �������ش�.
	{
		_x += _speed;
	}

}

//���� ���� ��
void player::flyInputKey()
{
	_FX = FLY_N;
	if (KEYMANAGER->isOnceKeyDown('A'))	// Ű�� ó�� ������ ������ �뽬
	{
		_isLeft = true;
	}
	else if (KEYMANAGER->isStayKeyDown('A'))	// �뽬 �� �Ŀ� �������� ���󰡴°� ����
	{
		_FX = FLY_L;
		changeState(FLY);						// ������Ʈ�� �ٲ� ���� index, count 0�����ʱ�ȭ���ִ� �Լ�.
	}
	if (KEYMANAGER->isOnceKeyDown('D'))	// Ű�� ó�� ������ ������ �뽬
	{
		_isLeft = false;
	}
	else if (KEYMANAGER->isStayKeyDown('D'))	// �뽬 �� �Ŀ� ���󰡴°� ����
	{
		_FX = FLY_R;
		changeState(FLY);						// ������Ʈ�� �ٲ� ���� index, count 0�����ʱ�ȭ���ִ� �Լ�.
	}
	_FY = FLY_N;
	if (KEYMANAGER->isStayKeyDown('W'))
	{
		_FY = FLY_U;
	}
	if (KEYMANAGER->isStayKeyDown('S'))
	{
		_FY = FLY_D;
	}
	if (_FX == NONE && _FY == NONE)
		changeState(FLY);						// ������Ʈ�� �ٲ� ���� index, count 0�����ʱ�ȭ���ִ� �Լ�.

}

float player::flyAngle(FLYING FX, FLYING FY)	
{
	float fAngle = 0;

	if (_FX == FLY_L)	// ����
	{
		if (_FY == FLY_U)
			fAngle = 135.f;
		else if (_FY == FLY_D)
			fAngle = 225.f;
		else
			fAngle = 180.f;
	}
	else if (_FX == FLY_R)	// ������ 
	{
		if (_FY == FLY_U)
			fAngle = 45.f;
		else if (_FY == FLY_D)
			fAngle = 315.f;
		else
			fAngle = 0.f;
	}
	else if (_FX == NONE)	// NONE�� �Ȱɾ� �ָ� Ű�� �ΰ� ������ �� �밢������ �Ȱ��� ���� �Ʒ��� �����δ�.
	{
		if (_FY == FLY_U)
		{
			fAngle = 90.f;
			changeState(FLY);			// ������Ʈ�� �ٲ� ���� index, count 0�����ʱ�ȭ���ִ� �Լ�.
		}
		if (_FY == FLY_D)
		{
			fAngle = 270.f;
			changeState(FLYDOWN);		// ������Ʈ�� �ٲ� ���� index, count 0�����ʱ�ȭ���ִ� �Լ�.
		}
	}


	_angle = fAngle * PI / 180.f;
	return _angle;
}

void player::flyMove()
{
	if (_FX != NONE)
	_x += cosf(_angle) * _flySpeed;

	_y += -sinf(_angle) * _flySpeed;
}

//���°� �ٲ𶧸��� �ε����� 0���� �ʱ�ȭ 
void player::changeState(int state)
{
	if (_state = state) return;	// �Ű������� �Լ��� ������ ��������.

	_state = state;
	_index = 0;
	_count = 0;
}


//�浹�Լ�
void player::collide()
{
	COLORREF color;
	int r;
	int g;
	int b;
	//���� �˻�
	//for (int i = _speed - 10 ; i < _speed + 10; i++)
	for(int i = _hitBox.top + _speed; i >= _hitBox.top; i--)
	{
		color = GetPixel(_mapPixel->getMemDC(), _x, i);
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);
	
		//isFly�϶� isFlyDown�� �ƴ� �� �浹 �˻�
		if ((r == 0 && g == 0 && b == 0)) // �������� �˻�
		{
			_y = i + (OTUS_HEIGTH / 2);
			break;
		}
	}	
	//�Ʒ� �˻�
	for (int i = _hitBox.bottom - _speed; i <= _hitBox.bottom ; i++)
	{
		color = GetPixel(_mapPixel->getMemDC(), _x, i);	// for���� ���鼭 _y�� i�� �˻�
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// ����Ÿ�� �ƴϸ� �˻�
		{
			_y = i - (OTUS_HEIGTH /2) ;	// _y�� i�� �˻��� �������� ���ͽ��� ����/2��ŭ �÷��ش�.
			_gravity = 0.f;
			
			_axisY = NONE;	// ��� �����ϴ°��� ����
			_FY = FLY_N;
			_jumpCount = 0;
			

			//if(_state == JUMPFALL)
			//_state >  IDLE , WALK, JUMP, JUMPFALL , FLY , FLYDOWN, ROLL
			if (_state != WALK && _state != ATK )
			{
				_state = IDLE;	// ���� IDLE�̿��� �ϴ��� , �ƴϸ� ���� IDLE�� �ٲ�� �ȵǴ��� �����غ���
			}

			//if (_state == FLY)
			//{
			//	_jumpCount = 0;
			//	//_state = IDLE;
			//}

			break;
		}
	}	
	//���� �˻�
	for (int i = _hitBox.left + _speed; i >= _hitBox.left; i--)
	{
		color = GetPixel(_mapPixel->getMemDC(), i, _y);	// for���� ���鼭 _x�� i�� �˻� 
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);
	
		if ((r == 0 && g == 0 && b == 0)) // �������̸� �˻�, �������� �˻�
		{
			_x = i + (OTUS_WIDTH / 2);
			break;
		}
	}
	//������ �˻�
	//for (int i = _hitBox.right; i < _hitBox.right + 1; i++)
	for (int i = _hitBox.right - _speed; i <= _hitBox.right; i++)
	{
		color = GetPixel(_mapPixel->getMemDC(), i, _y);
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);
		if ((r == 0 && g == 0 && b == 0))	// ����Ÿ�� �ƴϸ� �˻� ����Ÿ�� ����
		{
			_x = i - (OTUS_WIDTH / 2);
			break;
		}
	}
}

//�̹��������Ӽ���
void player::frameSetting()
{
	//�� ���캸�� (�����ϱ� )
	_count++;
	if (_isLeft)
		img[_state]->setFrameY(1);
	else if (!_isLeft)		// ������
		img[_state]->setFrameY(0);

	if (_count % 7 == 0)
	{
		_index++;
		if (_index > img[_state]->getMaxFrameX())
		{
			if (_state == ATK)
			{
				_state = _beforeState;
			}
			if (_state == JUMPFALL)
			{
				_index = img[_state]->getMaxFrameX() - 2;
			}
			else
				_index = 0;
		}
		img[_state]->setFrameX(_index);
	}
	
}
