#include "stdafx.h"
#include "player.h"
#include "enemyManager.h"		/*
									 ���漱���� enemyManagerŬ������ ���⼭ ������ ����ض�. include�����ָ� enemyMagager�� �ִ� ����� ���� ����� �� ����.
									 ��ȣ������ Ŭ����
								*/

/*
	�ڵ��� �� ���� ���� �� ����
	������ ��� �ɾ����� �� ������ ����
	�ɾ��� ������ �߸����� �ʾҴ��� �� ���캸��
*/

//�������� �����ε� ������� ����ٴϴ� ����

HRESULT player::init()
{
	//�÷��̾� ���� (�ε����� �̹��� ������)
	img[IDLE] = IMAGEMANAGER->findImage("IDLE");
	img[WALK] = IMAGEMANAGER->findImage("Walk");
	img[JUMP] = IMAGEMANAGER->findImage("Jump");
	img[JUMPFALL] = IMAGEMANAGER->findImage("Fall");
	img[FLY] = IMAGEMANAGER->findImage("FLY");
	img[FLYDOWN] = IMAGEMANAGER->findImage("FLYDOWN");
	img[ROLL] = IMAGEMANAGER->findImage("ROLL");
	img[ATK] = IMAGEMANAGER->findImage("ATTACK");
	img[LIFT] = IMAGEMANAGER->findImage("LIFT");	// ���ø��� ��� ������, ��Ŭ�� ���·� ���ƴٴϱ� ( �̰Ŵ� ��ü �ڿ��� ����ִ� ������ )
	img[LIFT2] = IMAGEMANAGER->findImage("LIFT2");	//	(��ü �տ��� ����ִ� ������)

	//�÷��̾� ü�� ��
	friendsFace = IMAGEMANAGER->findImage("FRIEND_UI");
	hpBarBack = IMAGEMANAGER->findImage("HP_BACK");
	hpBarFront = IMAGEMANAGER->findImage("HP_FRONT");

	_isLeft = false;			// true = ���� , false = ������
	_isFly = false;				// �����ִ��� �ƴ���
	_x = 440.f;					// �÷��̾� x��ǥ
	_y = 810.f;					// �÷��̾� y��ǥ
	_power = 0;					// ���ͽ��� �Ŀ� �ʱ�ȭ
	_speed = 7.0f;				// �÷��̾� �޸��� �ӵ� ( ���߿� ���¿� ���� �������� �� �ӵ�,���������� �ӵ�, ������ �ӵ� ��� ���¿� ���� �ӵ� �ٲ��ֱ� )
	_jumpSpeed = 15.f;			// �÷��̾� ���� �ӵ�
	_jumpCount = 0;
	_flySpeed = 9.f;
	_rollSpeed = 15.f;
	_angle = PI / 2;			// �÷��̾� ����
	_gravity = 0;				// �÷��̾� �߷�
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH); 
	_spinHitBox = RectMakeCenter(_x, _y, OTUS_WIDTH * 4, OTUS_HEIGTH / 3);	 // ���ͽ��� ���ʰ���(ȸ��) ��Ʈ�ڽ� 

	_count = _index = 0;		// �������̹��� ������ ���� �ʱ�ȭ

	_state = IDLE;				// �⺻ ���� = �������ִ� ���� (�÷��̾����)
	_axisX = _axisY = NONE;		// Ű�� �ȴ��� ���� (Ű ����)
	_FX = _FY = FLY_N;			// �������� �� Ű ���� �ʱ�ȭ NONE
	_beforeState = _state;
	_oldX = 0;
	_oldY = 0;					// ���� ��ġ

	_enemyManager = NULL;		// townScene�� ���� �� 0���� ������ش�. �ʱ�ȭ���ش�. (�����ָ� �����Ⱚ�� �����ͼ� ����)

	return S_OK;
}


void player::release()
{


}

void player::update()
{
	//���� ���� ��
	if(_isFly == true)
	{
		if (_state == ROLL)
			_speed = _rollSpeed;
		else
		_speed = 10.f;

	
		_axisY = NONE;
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
	}
	// �Ʒ� �����¿� �˻��ϱ����� ��Ʈ�ڽ��� ���� ���� ���ְ� �˻縦�Ѵ�.
	// �������� ��Ʈ 
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);	
	_spinHitBox = RectMakeCenter(_x, _y, OTUS_WIDTH * 4, OTUS_HEIGTH / 3);	 // ���ͽ��� ���ʰ���(ȸ��) ��Ʈ�ڽ� 
	//_spinHitBox = RectMakeCenter(_x, _y, OTUS_WIDTH * 4, OTUS_HEIGTH / 3);	 // ���ͽ��� ���ʰ���(ȸ��) ��Ʈ�ڽ� 
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
	if (_state == ATK)	// ��Ʈ�� �����Ҷ��� ��Ÿ����
	{
		Rectangle(getMemDC(), _spinHitBox.left - CAM->getX(), _spinHitBox.top - CAM->getY(), _spinHitBox.right - CAM->getX(), _spinHitBox.bottom - CAM->getY());
	}
	//img[_state]->setFrameY(_dir);
	img[_state]->frameRender(getMemDC(), _x - CAM->getX() - img[_state]->getFrameWidth() / 2, _y - CAM->getY() - img[_state]->getFrameHeight() / 2);

	//ü�� ��
	hpBarBack->render(getMemDC(), 120, 63);
	hpBarFront->render(getMemDC(), 120, 66);
	friendsFace->render(getMemDC(), 50, 50);

	char str[128];
	sprintf_s(str, "X��ǥ : %.f  Y��ǥ : %.f  ������ġ : %d �ε��� : %d  ���ǵ� : %.f   ���� : %.2f   x�� : %d   y�� : %d ���� : %d  ����ī��Ʈ : %d", _x, _y,_oldX, _index, _speed, _angle, _FX, _FY,_state, _jumpCount);
	TextOut(getMemDC(), 10, 10, str, strlen(str));
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Ű�Է��Լ� ( �Ұ�, ���������� �ٲ��ֱ� )
void player::groundInputKey()
{
	_axisX = NONE;										// Ű �Է��� ������ _axisX �� NONE�� �ȴ�.
	if (_state == JUMPFALL)								// �������� ���� �� W�� ������ �ٷ� �����ִ� ���°� �� �� �ְ� _jumpCount�� 1�� ���ְ�
		_jumpCount = 1;
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON) && _state != ATK )
	{
		changeState(ATK); // �ڵ带 ���ٷ� ������ �Լ��� ����µ� �޶����� ������ �Ű������� ���ش�.	
		_spinHitBox = RectMakeCenter(_x, _y, OTUS_WIDTH * 4, OTUS_HEIGTH / 3);	 // ���ͽ��� ���ʰ���(ȸ��) ��Ʈ�ڽ� 
	}
	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		changeState(LIFT);	// ��Ŭ���� �ϸ� ���ø��� ����� ���;��Ѵ�.
		_isFly = true;		// �����ִ� ���·� �ٲ�
	}
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE) && _state != ROLL && _state != JUMP)
	{
		if (_isLeft)
		EFFECTMANAGER->play("���������", _x, _y);	// ���� �������� ������ ���� ����Ʈ
		else 
		EFFECTMANAGER->play("�����������", _x, _y);	// ���� �������� ������ ���� ����Ʈ

		changeState(ROLL);
	}
	if (KEYMANAGER->isStayKeyDown('A'))	
	{
		_dir = LEFT;
		_isLeft = true;									// ������ ���� ���� true�� ���� �������� �����ش�.
		_axisX = LEFT;									// LEFT �������� �̵�
		if (_state == ATK)
			changeState(ATK);

		else if (_state == ROLL)
		{
			changeState(ROLL);
		}
		else if (_state != JUMP && _state != JUMPFALL && _state != ROLL)		// ������ �������� ������ �� �ٴ¸�� �ȳ����� ���ǿ� �ɾ��ش�.
			changeState(WALK);							// ������Ʈ�� �ٲ� ���� index, count 0�����ʱ�ȭ���ִ� �Լ�.
	}
	else if (KEYMANAGER->isStayKeyDown('D'))
	{
		_dir = RIGHT;
		_isLeft = false;								// ������ ���� ���� false�� ������ �������� �����ش�.
		_axisX = RIGHT;									// RIGHT�� ���������� �̵�
		if (_state == ATK)
			changeState(ATK);
		else if (_state != JUMP && _state != JUMPFALL && _state != ROLL)		// ������ �������� ������ �� �ٴ¸���� �ȳ����� ������ �ɾ��ش�.
			changeState(WALK);							// ������Ʈ�� �ٲ� ���� index, count 0�����ʱ�ȭ���ִ� �Լ�.
		
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
			_isFly = true;
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
	//���Ʒ��� ���ÿ� �浹������  , _oldX = _x ������ġx�� ������ġ X�� �����ϰ� 
	// ���� �Ʒ��� 
	_oldX = _x;
	_oldY = _y;									// ���� ��ġ(_oldY)�� ������ġ(_y)�� �����Ѵ�.
	if (_axisX == NONE && _axisY == NONE && _state != ATK && _state != ROLL && _state != JUMPFALL && _state != LIFT && _state != LIFT2)		// X,Y Ű�� �� �� NONE�̸� _state = IDLE �����̴�.
		_state = IDLE;

	/*
	_y�� 0�� �������ϱ�. ������ �߿��ϴ�.
	_gravity += 0.5f�� ���� ������ ���� ��Ƶ� 0�̾ƴ϶� 0.5f�� �������� JUMPFALL���°� �ż� �Ʒ��� ��������.
	_y�� ���� ������ ��� 0�� ��������.
	*/
	_y += _gravity;		// y�� �߷°��� ��� �����ش�. 
	_gravity += 0.5f;	// �Ʒ��� ������ �� �ְ� ������ �ƴϾ ��� �߷°��� �����ش�.

	if (_axisY == UP)	// _axisY == UP�� �� ���� ������ �����ش�.	
		_y += -sinf(PI / 2) * _jumpSpeed;
	
	/* 
	-sinf(angle) * speed << ������
	_gravity += -sinf(angle) * speed; << �̰Ÿ� ��� ���ϸ� �Ʒ��� �������°� �ƴ϶� ������ ������ ��
	�׷��� �ѹ��� �����ϰ� ������ �ɾ���� ��
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

	if (_state == ROLL)				// ������ ������ ��
	{
		_speed = _rollSpeed;		// ���ǵ�� �����½��ǵ�
		if (_isLeft)				// ������ ��
		{
			_x -= _speed;			// �������� ���ǵ常ŭ
		}
		else if (!_isLeft)			// ���������� ���ǵ常ŭ
		{
			_x += _speed;
		}
	}
	else		// ������ ���°� �ƴ϶�� ���ǵ�� 7�̰� �����¼ӵ��� �ȱ�ӵ��� ���������ʰ� ���ش�.
	{
		_speed = 7.0f;				

		if (_axisX == LEFT)		//  _axisX == LEFT�� �� �������� �������ش�.
		{
			_x -= _speed;
		}
		else if (_axisX == RIGHT)	//_axisX == RIGHT�� �� ���������� �������ش�.
		{
			_x += _speed;
		}

	}
	
}

//���� ���� ��
void player::flyInputKey()
{
	if (_state != ROLL)
		_FX = FLY_N;

	if (_FX == NONE && _FY == NONE && _state != ATK && _state != ROLL && _state != LIFT && _state != LIFT2)	// _FX,_FY�� NONE�̰� _state�� ATK�ƴϰ� ROLL�� �ƴ� �� FLY���� 
		changeState(FLY);						// ������Ʈ�� �ٲ� ���� index, count 0�����ʱ�ȭ���ִ� �Լ�.

	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		changeState(ATK);
	}
	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		changeState(LIFT);
	}
	else if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		// ���콺�� ������������ ���ͽ��� ���ۺ��� ���鼭 ������ �̹������ ���߿� �߰��ϸ� �ֱ�
	}
	if (KEYMANAGER->isStayKeyDown('A'))	// �뽬 �� �Ŀ� �������� ���󰡴°� ����
	{
		_isLeft = true;
		_FX = FLY_L;
		if (_state == ATK)
			changeState(ATK);
		else if (_state == ROLL)
			changeState(ROLL);
		else if (_state == LIFT)
			changeState(LIFT);
		else
			changeState(FLY);						// ������Ʈ�� �ٲ� ���� index, count 0�����ʱ�ȭ���ִ� �Լ�.
	}
	if (KEYMANAGER->isStayKeyDown('D'))	// �뽬 �� �Ŀ� ���󰡴°� ����
	{
		_isLeft = false;
		_FX = FLY_R;
		if (_state == ATK)
			changeState(ATK);
		else if (_state == ROLL)
			changeState(ROLL);
		else if (_state == LIFT)
			changeState(LIFT);
		else
			changeState(FLY);						// ������Ʈ�� �ٲ� ���� index, count 0�����ʱ�ȭ���ִ� �Լ�.
	}
	if(_state != ROLL)
		_FY = FLY_N;
	if (KEYMANAGER->isStayKeyDown('W'))
	{
		_FY = FLY_U;
		if (_state == ATK)
			changeState(ATK);
		else if (_state == ROLL)
			changeState(ROLL);
		else if (_state == LIFT)
			changeState(LIFT);
		else
			changeState(FLY);			// ������Ʈ�� �ٲ� ���� index, count 0�����ʱ�ȭ���ִ� �Լ�.
	}
	if (KEYMANAGER->isStayKeyDown('S'))
	{
		_FY = FLY_D;
		if (_state == ATK)
			changeState(ATK);
		else if (_state == ROLL)
			changeState(ROLL);
		else if (_state == LIFT)
			changeState(LIFT);
		else
			changeState(FLYDOWN);			// ������Ʈ�� �ٲ� ���� index, count 0�����ʱ�ȭ���ִ� �Լ�.
	}
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE) && _state != ROLL)
	{
		changeState(ROLL);
		if (_FX != FLY_N )
		{
			if (_isLeft)
				EFFECTMANAGER->play("���������", _x, _y);	// ���� �������� ������ ���� ����Ʈ
			else
				EFFECTMANAGER->play("�����������", _x, _y);	// ���� �������� ������ ���� ����Ʈ
		}
	}
}

float player::flyAngle(FLYING FX, FLYING FY)	
{
	float fAngle = 0;

	//������������ Ű�� �ȴ����� ������ �����ǰ�
	if (_state == ROLL)
	{
		if (_isLeft)
			fAngle = 180.f;
		else
			fAngle = 0.f;
	}

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
		}
		if (_FY == FLY_D)
		{
			fAngle = 270.f;
		}
	}
	


	_angle = fAngle * PI / 180.f;
	return _angle;
}

void player::flyMove()
{
	_oldX = _x;
	if (_state == ROLL)							// ������ ������ ���� flySpeed�� rollSpeed�� �ǰ�
	{
		_flySpeed = _rollSpeed;

		_x += cosf(_angle) * _flySpeed;
		_y += -sinf(_angle) * _flySpeed;
	}
	else
	{
		_flySpeed = 10.f;
		if (_FX != NONE)	 // �����ָ� ��� �����δ�.
			_x += cosf(_angle) * _flySpeed;

		_y += -sinf(_angle) * _flySpeed;
	}
}

//���°� �ٲ𶧸��� �ε����� 0���� �ʱ�ȭ 
void player::changeState(int state)
{
	if (_state == state) return;	// �Ű������� �Լ��� ������ ��������.

	_state = state;
	_index = 0;
	_count = 0;
}


//�浹�Լ�
void player::collide()
{
	//�浹ó���Լ� ( �� , ���Ͷ� �÷��̾� )
	this->collideMap();
	this->collideActor();
}

void player::collideMap()
{
	bool upCollision = false;	// ���Ʒ��� �浹�ߴµ� x�� ������ ���� ���׳����ʰ� ������ġ�� �̵������ش�
	COLORREF color;
	int r;
	int g;
	int b;
	//���� �˻�
	for (int i = _hitBox.top + _speed; i >= _hitBox.top; i--)
	{
		color = GetPixel(_mapPixel->getMemDC(), _x, i);
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);

		if ((r == 0 && g == 0 && b == 0)) // �������� �˻�
		{
			if (!_isFly)
				_x = _oldX;
			_y = i + (OTUS_HEIGTH / 2);
			_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);
			upCollision = true;
			break;
		}
	}
	//�Ʒ� �˻�
	for (int i = _hitBox.bottom - _speed; i <= _hitBox.bottom; i++)
	{
		color = GetPixel(_mapPixel->getMemDC(), _x, i);	// for���� ���鼭 _y�� i�� �˻�
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);

		if (_state == JUMPFALL || _state == FLYDOWN || _state == WALK || _state == ROLL || _state == ATK || _state == IDLE)	// �Ʒ����� ���� �ö󰥶� �˻��ϸ� �ȵǴϱ� isFly�� false�̰ų� �Ʒ��� �����ö��� �˻� �Ѵ�.
		{
			if (!(r == 255 && g == 0 && b == 255))	// ����Ÿ�� �ƴϸ� �˻�
			{

				_y = i - (OTUS_HEIGTH / 2);	// _y�� i�� �˻��� �������� ���ͽ��� ����/2��ŭ �÷��ش�.
				_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);

				_gravity = 0.f;				// �߷��� 0���� �ٲ��ְ�		
				_axisY = NONE;	// ��� �����ϴ°��� ����
				_FY = FLY_N;	//
				_jumpCount = 0;	// ���� �����ϱ� ����ī��Ʈ 0���� �ٲ��ش�.
				_isFly = false;	// ���� �ʴ� ���¶�°� �ٲ��ֱ�
				if (_state != ROLL)
					_speed = 7;
				if (_state != WALK && _state != ATK && _state != ROLL && _state != LIFT)	// �ٴ»��°� �ƴϰ� ���ݻ��°� �ƴϸ� IDLE
				{
					_state = IDLE;	// ���� IDLE�̿��� �ϴ��� , �ƴϸ� ���� IDLE�� �ٲ�� �ȵǴ��� �����غ���
				}
				break;
			}
		}
		else if (_state == FLY && upCollision && !(r == 255 && g == 0 && b == 255)) _x = _oldX;

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
			_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);

			break;
		}
	}
	//������ �˻�
	for (int i = _hitBox.right - _speed; i <= _hitBox.right; i++)
	{
		color = GetPixel(_mapPixel->getMemDC(), i, _y);
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);
		if ((r == 0 && g == 0 && b == 0))	// ����Ÿ�� �ƴϸ� �˻� ����Ÿ�� ����
		{
			_x = i - (OTUS_WIDTH / 2);
			_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);

			break;
		}
	}
}

// �÷��̾�� ������ �浹 ó��
void player::collideActor()
{
	//�÷��̾ ���Ϳ��� ������� �ٰ�
	//_enemyManager->getVEnemy()[0]->getHitbox(); // ���ʹ̸Ŵ����� �ִ� 0�� ���ʹ��� ��Ʈ�ڽ� , �Ǵ� ���ʹ��� �������� ������ �ҷ��� �� �ִ�.
	//1. ���ʹ̸Ŵ����� ���ʹ̸� �������´�.
	//2. ��� ���������� �����ͼ� ������.
	if (_enemyManager != NULL)
	{
		for (int i = 0; i < _enemyManager->getVEnemy().size(); i++)
		{
			RECT temp;
			if (IntersectRect(&temp, &_spinHitBox, &_enemyManager->getVEnemy()[i]->getHitbox()))	// �÷��̾��� ���� ��Ʈ�� ���ʹ��� ��Ʈ�ڽ��� �˻��Ѵ�.
			{
				if (_state == ATK)
				{
					_enemyManager->getVEnemy()[i]->damaged(this);	// ���ʹ̸Ŵ����� i��°�� ���ʹ̿��� �÷��̾��� ������ �Ѱ����.
				}
			}
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
			if (_state == ATK || _state == ROLL)
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
