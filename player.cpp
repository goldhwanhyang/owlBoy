#include "stdafx.h"
#include "player.h"
#include "enemyManager.h"		
#include "stuffManager.h"

/*
	 ���漱���� enemyManagerŬ������ ���⼭ ������ ����ض�. include�����ָ� enemyMagager�� �ִ� ����� ���� ����� �� ����.
	 ��ȣ������ Ŭ����
*/

/*
	�ڵ��� �� ���� ���� �� ����
	������ ��� �ɾ����� �� ������ ����
	�ɾ��� ������ �߸����� �ʾҴ��� �� ���캸��
*/

/*
	���콺 ��Ŭ���� ������������ �������� �׷��ִ°�
	WALK�� �� �밢������ �������°�
	
*/

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
	img[HIT] = IMAGEMANAGER->findImage("DAMAGED");									// ���ͽ� �´¸�� 
	img[DEAD] = IMAGEMANAGER->findImage("DEAD");
	img[LIFT] = IMAGEMANAGER->findImage("LIFT");									// ���ø��� ��� ������, ��Ŭ�� ���·� ���ƴٴϱ� ( �̰Ŵ� ��ü �ڿ��� ����ִ� ������ )
	for (int i = 0; i < STATE_MAX; ++i)
	{
		_frameCount[i] = 7;
	}
	_frameCount[ROLL] = 5;
	_frameCount[ATK] = 5;
	_frameCount[FLY] = 8;
	_liftImg = IMAGEMANAGER->findImage("LIFT2");									//	(��ü �տ��� ����ִ� ������)

	//hp��
	friendsFace = IMAGEMANAGER->findImage("FRIEND_UI");
	if (_hpBar == NULL)
	{
		_hpBar = new progressBar;
		_hpBar->init("HP_FRONT", "HP_BACK", 120, 63, 220, 30);
		_maxHp = 30;	// �ƽ�HP
		_hp = 15;	// ���� ������(�̹�������������29)
		_hpBar->setGauge(_hp, _maxHp);
	}
	//����
	_maxCoin = 9999;
	_coin = 0;

	_isDead = false;
	_isLeft = false;																// true = ���� , false = ������
	_isFly = false;																	// �����ִ��� �ƴ���
	_isKnockBack = false;															// �˹����� �ƴ���
	_isBack = false;																// �����ϰ� �ڷιз�����
	_x = 0.f;																		// �÷��̾� x��ǥ
	_y = 0.f;																		// �÷��̾� y��ǥ
	_z = 10;
	_power = 0;																		// ���ͽ��� �Ŀ� �ʱ�ȭ
	_speed = 7.0f;																	// �÷��̾� �޸��� �ӵ� ( ���߿� ���¿� ���� �������� �� �ӵ�,���������� �ӵ�, ������ �ӵ� ��� ���¿� ���� �ӵ� �ٲ��ֱ� )	
	_jumpSpeed = 15.f;																// �÷��̾� ���� �ӵ�
	_jumpCount = 0;																	// ���� ī��Ʈ
	_flySpeed = 9.f;																// ���� �ӵ�
	_rollSpeed = 15.f;																// ������ �ӵ�
	_knockBackSpeed = 10.f;															// �ڷ� �з�����(�˹�)�ӵ�
	_backSpeed = 20.0f;																// �ڷ� �з����� �ӵ�
	_angle = PI / 2;																// �÷��̾� ����
	_gravity = 0;																	// �÷��̾� �߷�
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);						//���ͽ��� ������Ʈ�ڽ�
	_spinHitBox = RectMakeCenter(_x, _y, OTUS_WIDTH * 4, OTUS_HEIGTH / 3);			// ���ͽ��� ���ʰ���(ȸ��) ��Ʈ�ڽ� 
	_rollHitBox = RectMakeCenter(_x, _y, OTUS_WIDTH * 3, OTUS_HEIGTH * 1.2);		//���ͽ��� ������ ��Ʈ�ڽ�

	_count = _index = 0;															// �������̹��� ������ ���� �ʱ�ȭ

	_state = IDLE;																	// �⺻ ���� = �������ִ� ���� (�÷��̾����)
	_axisX = _axisY = NONE;															// Ű�� �ȴ��� ���� (Ű ����)
	_FX = _FY = FLY_N;																// �������� �� Ű ���� �ʱ�ȭ NONE
	_beforeState = _state;															// ���� ���¿� ���� ���¸� ����
	_oldX = 0;																		// ���� X��ġ
	_oldY = 0;																		// ���� Y��ġ
	_walkY = 0;

	_enemyManager = NULL;															// townScene�� ���� �� 0���� ������ش�. �ʱ�ȭ���ش�. (�����ָ� �����Ⱚ�� �����ͼ� ����)
	_liftableActor = NULL;															// �ʱ�ȭ ( �����ָ� ���� )

	return S_OK;
}


void player::release()
{
	if (_hpBar != NULL)
	{
		_hpBar->release();
		SAFE_DELETE(_hpBar);
	}
}

void player::update()
{
	if (_state != HIT && _state != DEAD)																// �˹���°� �ƴ� �� (�浹�� �ƴ� ��)
	{
		if (_isFly == true)															// _isFly�� true�̸�, �����ִ� ���¸�
		{
			if (_state == ROLL)														// �����ִ� �߿� ������ ���°� �Ǹ�
				_speed = _rollSpeed;												// ���ǵ� = ������ ���ǵ��
			else
				_speed = 10.f;														// �׷��� ������ �������� �� �ӵ��� 10�̴�.

			_axisY = NONE;															// ������ _isFly�� �ϸ� _axisY�� ������ ���·� �����ż� NONE���� �ٲ��ش�.
			_gravity = 0;															// �������� �ʰ� 0���� ����������
			flyInputKey();															// �������� �� Ű �Է�
			flyAngle(_FX, _FY);														// �Է¹��� Ű�� ���� ���� �������ֱ�
			flyMove();																// ������ ������ ���� �����δ�.
		}

		else																		// ���� ���� ��
		{
			_speed = 7.0f;															// ���� ���� �� ���ǵ� 7.0f
			groundInputKey();														// ���� ���� �� �Է��ϴ� Ű
			groundAxis(_axisX, _axisY);												// ���¿� ���� �������ֱ�
		}

		commonInputKey();

		if (_liftableActor != NULL)
		{
			changeState(LIFT);
			_liftableActor->lifted(this);
		}
	}
	// �Ʒ� �����¿� �˻��ϱ����� ��Ʈ�ڽ��� ���� ���� ���ְ� �˻縦�Ѵ�.
	// �������� ��Ʈ 
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);	
	_spinHitBox = RectMakeCenter(_x, _y, OTUS_WIDTH * 4, OTUS_HEIGTH / 3);			 // ���ͽ��� ���ʰ���(ȸ��) ��Ʈ�ڽ� 
	_rollHitBox = RectMakeCenter(_x, _y, OTUS_WIDTH * 3, OTUS_HEIGTH * 1.2);
	//�浹�Լ�
	this->collide();
	//ü�¹� ������Ʈ
	_hpBar->update();
	_hpBar->setGauge(_hp, _maxHp);
	//�ƿﺸ�� ������ ������
	this->frameSetting();
}

void player::render()
{
	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		Rectangle(getMemDC(), _rollHitBox.left - CAM->getX(), _rollHitBox.top - CAM->getY(), _rollHitBox.right - CAM->getX(), _rollHitBox.bottom - CAM->getY());
		Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());		
	}
	if (_state == ATK)	// ��Ʈ�� �����Ҷ��� ��Ÿ����
	{
		//Rectangle(getMemDC(), _spinHitBox.left - CAM->getX(), _spinHitBox.top - CAM->getY(), _spinHitBox.right - CAM->getX(), _spinHitBox.bottom - CAM->getY());
	}
	img[_state]->frameRender(getMemDC(), _x - CAM->getX() - img[_state]->getFrameWidth() / 2, _y - CAM->getY() - img[_state]->getFrameHeight() / 2);

	char str[128];
	//sprintf_s(str, "X��ǥ : %.f  Y��ǥ : %.f  �߷� : %.f �ε��� : %d  ���ǵ� : %.f   ���� : %.2f   x�� : %d   y�� : %d ���� : %d  ����ī��Ʈ : %d", _x, _y,_gravity, _index, _speed, _angle, _FX, _FY,_state, _jumpCount);
	sprintf_s(str, " ���� : %d", _coin);
	TextOut(getMemDC(), 10, 10, str, strlen(str));
	
	if (_liftableActor != NULL)
	{
		_liftImg->setFrameX(img[_state]->getFrameX());
		_liftImg->setFrameY(img[_state]->getFrameY());
		_liftImg->setX(_x);
		_liftImg->setY(_y);
	}

	_hpBar->render();
	friendsFace->render(getMemDC(), 50, 50);

	//õ���ڸ� 1234 / 1000 = 1.234�ϱ� 1�� ����
	IMAGEMANAGER->findImage("number")->setFrameX(_coin / 1000);
	IMAGEMANAGER->findImage("number")->frameRender(getMemDC(), 50, 150);
	
	//���� �ڸ� 
	IMAGEMANAGER->findImage("number")->setFrameX(((_coin % 1000)/100));
	IMAGEMANAGER->findImage("number")->frameRender(getMemDC(), 70, 150);

	//�����ڸ�
	IMAGEMANAGER->findImage("number")->setFrameX((_coin % 100)/10);
	IMAGEMANAGER->findImage("number")->frameRender(getMemDC(), 90, 150);
	
	//�����ڸ�
	IMAGEMANAGER->findImage("number")->setFrameX(_coin % 10);
	IMAGEMANAGER->findImage("number")->frameRender(getMemDC(), 110, 150);

	//(_coin  % 100) /10
	//	(_coin % 10)
	/*
	1234 / 1000 == 1.234 >> 1�� ������
	(1234 % 1000) / 100 >> 2�� ������
	1234 / 100 == 12.34
	1234 % 100 == 34
	1280 / 100 == 1.280
	1280 % 100 == 280
	(1280 % 100) / 10 28
	*/
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Ű�Է��Լ� ( �Ұ�, ���������� �ٲ��ֱ� )
void player::groundInputKey()
{
	_axisX = NONE;																	// Ű �Է��� ������ _axisX �� NONE�� �ȴ�.
	if (_state == JUMPFALL)															// �������� ���� �� W�� ������ �ٷ� �����ִ� ���°� �� �� �ְ� _jumpCount�� 1�� ���ش�.
		_jumpCount = 1;

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE) && _state != ROLL && _liftableActor == NULL)
	{
		changeState(ROLL);
		if (_state == ROLL)
		{			
			if (_isLeft)
				EFFECTMANAGER->play("���������", _x, _y);							// ���� �������� ������ ���� ����Ʈ
			else
				EFFECTMANAGER->play("�����������", _x, _y);							// ���� �������� ������ ���� ����Ʈ		

			SOUNDMANAGER->play("������", _soundVolume);
		}
		_rollHitBox = RectMakeCenter(_x, _y, OTUS_WIDTH * 3, OTUS_HEIGTH * 1.2);
	}
	if (KEYMANAGER->isStayKeyDown('A'))	
	{
		_dir = LEFT;
		_isLeft = true;																// ������ ���� ���� true�� ���� �������� �����ش�.
		_axisX = LEFT;																// LEFT �������� �̵�
		if (_state == ATK)
			changeState(ATK);
		else if (_state == ROLL)
		{
			changeState(ROLL);
		}
		else if (_state != JUMP && _state != JUMPFALL && _state != ROLL)			// ������ �������� ������ �� �ٴ¸�� �ȳ����� ���ǿ� �ɾ��ش�.
			changeState(WALK);														// ������Ʈ�� �ٲ� ���� index, count 0�����ʱ�ȭ���ִ� �Լ�.
	}
	else if (KEYMANAGER->isStayKeyDown('D'))
	{
		_dir = RIGHT;
		_isLeft = false;															// ������ ���� ���� false�� ������ �������� �����ش�.
		_axisX = RIGHT;																// RIGHT�� ���������� �̵�
		if (_state == ATK)
			changeState(ATK);
		else if (_state != JUMP && _state != JUMPFALL && _state != ROLL)			// ������ �������� ������ �� �ٴ¸���� �ȳ����� ������ �ɾ��ش�.
			changeState(WALK);														// ������Ʈ�� �ٲ� ���� index, count 0�����ʱ�ȭ���ִ� �Լ�.
		
	}
	if (_state != JUMP )															// Ű�� �ȴ����� �ٷ� NONE�� �ż� ������ �ȵ�
		_axisY = NONE;
	if (KEYMANAGER->isOnceKeyDown('W'))
	{
		if (_state == ATK)
			changeState(ATK);	
		else		
			changeState(JUMP);														// ���� �����ϴ� �������� �����ش�.
		
		_axisY = UP;																// UP�� �Ǹ� ���� ������ �Ѵ�.
		_jumpCount += 1;															// _jumpCount�� 2�� �Ǹ� FLY���·� �ٲ��ش�.
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
		_gravity -= 0.16f;															// WŰ�� ���� ������ ���� ���� ���� ������ �� �ִ�.
	}
	
}

// ���� �� �κ�
void player::groundAxis(WAY axisX, WAY axisY)										// Ű �Է����� �ٲ��� ���³� �Ұ��� ���� �������ֱ�
{
	//���Ʒ��� ���ÿ� �浹������  , _oldX = _x ������ġx�� ������ġ X�� �����ϰ� 
	_oldX = _x;
	_oldY = _y;									// ���� ��ġ(_oldY)�� ������ġ(_y)�� �����Ѵ�.
	if (_axisX == NONE && _axisY == NONE && _state != ATK && _state != ROLL && _state != JUMPFALL && _state != LIFT && _state != HIT && _state != DEAD )		// X,Y Ű�� �� �� NONE�̸� _state = IDLE �����̴�.
		_state = IDLE;

	/*
	_y�� 0�� �������ϱ�. ������ �߿��ϴ�.
	_gravity += 0.5f�� ���� ������ ���� ��Ƶ� 0�̾ƴ϶� 0.5f�� �������� JUMPFALL���°� �ż� �Ʒ��� ��������.
	_y�� ���� ������ ��� 0�� ��������.
	*/
	if (_state != ROLL)
	{
		if (_gravity > _jumpSpeed * 2)
			_gravity = _jumpSpeed * 2;
		_y += _gravity;		// y�� �߷°��� ��� �����ش�. 
		_gravity += 0.5f;	// �Ʒ��� ������ �� �ְ� ������ �ƴϾ ��� �߷°��� �����ش�.
		if (_state == ATK)
		{
			_gravity -= 0.3f;
		}
	}
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
		//�����߿� �ٷ� jumpfall�� �ȵǰ�
		//if (_state != ROLL && _state != ATK && _state != WALK)
		if (_state != ROLL && _state != ATK)
		{
			// �ѹ��� �����������Ѵ�.
			// ������ �ߴٰ� �������� �ڿ������� �������� ���� �߷��� ����
			if (_state == JUMP)
				_gravity += -sinf(PI / 2) * _jumpSpeed;
			changeState(JUMPFALL);			// ������Ʈ�� �ٲ� ���� index, count 0�����ʱ�ȭ���ִ� �Լ�.
		}
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
	else																					// ������ ���°� �ƴ϶�� ���ǵ�� 7�̰� �����¼ӵ��� �ȱ�ӵ��� ���������ʰ� ���ش�.
	{
		_speed = 7.0f;					
		if (_axisX == LEFT)																	//  _axisX == LEFT�� �� �������� �������ش�.
		{
			_x -= _speed;
		}
		else if (_axisX == RIGHT)															//_axisX == RIGHT�� �� ���������� �������ش�.
		{
			_x += _speed;
		}
	}
}

//���� ���� ��
void player::flyInputKey()
{
	if (_state != ROLL)																		// ������ ���°� �ƴ� ���� _FX�� NONE���� ���ش�. 
		_FX = FLY_N;			

	if (_FX == NONE && _FY == NONE && _state != ATK && _state != ROLL && _state != LIFT)	//_FX,_FY�� NONE�̰� _state�� ����,������,����ִ� ���°� �ƴ� �� FLY
		changeState(FLY);																	// ������Ʈ�� �ٲ� ���� index, count 0�����ʱ�ȭ���ִ� �Լ�.

	
	if (KEYMANAGER->isStayKeyDown('A'))														// �뽬 �� �Ŀ� �������� ���󰡴°� ����
	{
		_isLeft = true;																		// _isLeft = true�� ���� �������� �����ش�.
		_FX = FLY_L;																		// _FX �� ������ �������� �˷��ش�.
		if (_state == ATK)																	// A�� �����µ� ���ݻ��¸� ���� ����� �����ְ� ROLL�̸� ROLL, LIFT�� LIFT �׷��� ������ FLY
			changeState(ATK);
		else if (_state == ROLL)
			changeState(ROLL);
		else if (_state == LIFT)
			changeState(LIFT);
		else
			changeState(FLY);																
	}
	if (KEYMANAGER->isStayKeyDown('D'))														
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
			changeState(FLY);																// ������Ʈ�� �ٲ� ���� index, count 0�����ʱ�ȭ���ִ� �Լ�.
	}
	if(_state != ROLL)																		// ������ ���°� �ƴҶ� _FY�� NONE���� �ٲ��ش�.
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
			changeState(FLY);								
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
			changeState(FLYDOWN);
	}
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE) && _state != ROLL)								// ������ ���߿� �ٽñ����� ���ϰ� �ɾ��ش�.
	{
		changeState(ROLL);
		SOUNDMANAGER->play("������", _effectVolume);
		if (_FY == NONE && _liftableActor == NULL)																	// ���Ʒ��� ������ ����������Ʈ�� �ȳ����� ���ش�.
		{			
			if (_isLeft)
				EFFECTMANAGER->play("���������", _x, _y);									// ���� �������� ������ ���� ����Ʈ
			else
				EFFECTMANAGER->play("�����������", _x, _y);									// ���� �������� ������ ���� ����Ʈ		
		}
	}
}

float player::flyAngle(FLYING FX, FLYING FY)	
{
	float fAngle = 0;

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
	else if (_FX == NONE)											// NONE�� �Ȱɾ� �ָ� Ű�� �ΰ� ������ �� �밢������ �Ȱ��� ���� �Ʒ��� �����δ�.
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
	_oldX = _x;														// _oldX(������ġ)�� _x(������ġ)�� �����Ѵ�. 
	if (_state == ROLL)												// ������ ���¸�
	{
		_flySpeed = _rollSpeed;										// ������ ������ ���� flySpeed�� rollSpeed�� �ǰ�

		_x += cosf(_angle) * _flySpeed;
		_y += -sinf(_angle) * _flySpeed;
	}
	else
	{
		_flySpeed = 10.f;
		if (_FX != NONE)											// �����ָ� ��� �����̴ϱ� _FXŰ�� ������ ���� �����̰� ���ش�.
			_x += cosf(_angle) * _flySpeed;
		_y += -sinf(_angle) * _flySpeed;
	}
}

//�����ϱ�
void player::changeState(int state)									// ����(������Ʈ)�� �ٲ� ���� index, count 0�����ʱ�ȭ���ִ� �Լ�.
{
	if (_state == state) return;									// �Ű������� �Լ��� ������ ��������.	

	if (state != ATK && state != ROLL)								// �������� �� �����̳� �����⸦ �ϸ� _isFly�� falseó���� �ȵǰ�
	{
		if (state == FLY || state == FLYDOWN || state == LIFT)		// state�� FLY�̰ų� FLYDOWN�̰ų� LIFT�϶� _isFly�� false �ȵǰ� ������ �ɾ��ش�.
			_isFly = true;
		else														// state�� FLY�� �ƴϰ� FLYDOWN�� �ƴϰ� LIFT�� �ƴϸ� false
			_isFly = false;
	}
	// _state�� ������ �� _beforeState�� ������ ���� state�� �������϶� _state�� �����Ⱑ ���� _beforState�� ����,�����⸦ �ݺ��ϴϱ� ����������Ѵ�.
	// ���� �����Ⱑ �ƴҶ��� �������¸� �����ϸ� �ݺ����� �ʴ´�.
	if (_state != ATK && _state != ROLL && _state != LIFT && _state != DEAD)
	{
		_beforeState = _state;										// ������->������ �ϸ� �ƹ��͵� �Է� ���ߴµ� ��������ݸ� �ݺ��ϱ� ������ �ɾ��� ����. 
	}
	_state = state;

	_index = 0;
	_count = 0;
}

//�����ϱ�
void player::commonInputKey()														 // ������������ ���� ���� �� ���� Ű �Է�
{																					
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON) && _state != ATK)
	{							
		SOUNDMANAGER->play("����", _effectVolume);
		if (_liftableActor == NULL)													// _liftableActor�� NULL�̸� ���ͽ��� �ƹ��͵� �ȵ�� �ִ� �����̱⶧���� ���ͽ��� �����Ѵ�.											
		{																			
			changeState(ATK);														 // �ڵ带 ���ٷ� ������ �Լ��� ����µ� �޶����� ������ �Ű������� ���ش�.	
			_gravity = 0.f;															 // ���� ���� ���� ���� ���� �� �߷��� ������ 0
			_spinHitBox = RectMakeCenter(_x, _y, OTUS_WIDTH * 4, OTUS_HEIGTH / 3);	 // ���ͽ��� ���ʰ���(ȸ��) ��Ʈ�ڽ� 
		}
		else
		{
			_liftableActor->attack();												 // _liftableActor��NULL�� �ƴϸ� �Ե�? ����ִ� �����? �����Ѵ�.
			_liftableActor = _liftableActor->use(this);
			changeState(FLY);
		}
	}
	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		if (_liftableActor != NULL)													// _liftableActor�� NULL�� �ƴϸ� ���ͽ��� ���� ����ִ� �����̴�.
		{
			//�� ���콺�� ������������ ������ ���ؼ� ������ �տ� �ƹ��͵� ���ٴ°��� �˸������� NULL�� �ٲ��ְ� ���µ� FLY�� �ٲ��ش�.
			SOUNDMANAGER->play("������", _effectVolume);	
			_liftableActor->throwed(10, getAnglef(_x - CAM->getX(),_y - CAM->getY() , _ptMouse.x, _ptMouse.y));

			_liftableActor = NULL;
			changeState(FLY);
		}
		else												// NULL�̸�
		{
			collideStuff();
			if (_liftableActor != NULL)
			{
				changeState(LIFT);
				if (_liftableActor == _geddy)	// �Ե��϶��� ��ȯ����, ��� ����Ʈ 
				{
					SOUNDMANAGER->play("��ȯ", _effectVolume);
					EFFECTMANAGER->play("���", _x, _y);
				}				

				_liftableActor->lifted(this);
			}
		}
	}
	else if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))	// ��Ŭ���� ���������� �� ������ �׷��ֱ�
	{
		if (_liftableActor != NULL)
		{

		}
	}
	if (KEYMANAGER->isOnceKeyDown('Q'))	// �Ե� ��ȯ
	{
		if (_liftableActor == NULL ) 
		{
			SOUNDMANAGER->play("��ȯ", _effectVolume);
			EFFECTMANAGER->play("��ȯ", _x, _y);
			EFFECTMANAGER->play("��ȯ", _geddy->getX(), _geddy->getY());
			_liftableActor = _geddy;								// NULL�̴ϱ� geddy�� ����ִٴ°��� �� �� �ִ�.
			_liftableActor->setIsActive(TRUE);
		}			
	}
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
	/*
		_hitBox.top = _y - (OTUS_HEIGHT / 2)
		_hitBox.bottp = _y + (OTUS_HEIGHT / 2)
			
		_y = _hitBox.top + (OTUS_HEIGHT / 2)
			 _hitBox.bottop - (OTUS_HEIGHT / 2)

		_y = __hitBox.top  
	*/

	//���� �˻�
	//for (int i = _hitBox.top + _speed; i >= _hitBox.top; i--)
	for (int i = _y - (OTUS_HEIGTH / 2) + _speed; i >= _y - (OTUS_HEIGTH / 2); i--)
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
			//_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);
			upCollision = true;
			break;
		}
	}
	//�Ʒ� �˻�
	//for (int i = _hitBox.bottom - _speed; i <= _hitBox.bottom; i++)
	//walk���� �϶��� ���� �� �Ʒ��� �˻��Ѵ�.
	if (_state == WALK)
		_walkY = 20;
	else
		_walkY = 0;

	for (int i = _y + (OTUS_HEIGTH /2) - _speed; i <= _y + (OTUS_HEIGTH / 2) + _walkY; i++)
	{
		color = GetPixel(_mapPixel->getMemDC(), _x, i);	// for���� ���鼭 _y�� i�� �˻�
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);

		if (_state == JUMPFALL || _state == FLYDOWN || _state == WALK || _state == ROLL || _state == ATK || _state == IDLE )	// �Ʒ����� ���� �ö󰥶� �˻��ϸ� �ȵǴϱ� isFly�� false�̰ų� �Ʒ��� �����ö��� �˻� �Ѵ�.
		{
			if (!(r == 255 && g == 0 && b == 255))	// ����Ÿ�� �ƴϸ� �˻�
			{
				_y = i - (OTUS_HEIGTH / 2);	// _y�� i�� �˻��� �������� ���ͽ��� ����/2��ŭ �÷��ش�.
				//_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);

				_gravity = 0.f;				// �߷��� 0���� �ٲ��ְ�		
				_axisY = NONE;	// ��� �����ϴ°��� ����
				_FY = FLY_N;	//
				_jumpCount = 0;	// ���� �����ϱ� ����ī��Ʈ 0���� �ٲ��ش�.
				_isFly = false;	// ���� �ʴ� ���¶�°� �ٲ��ֱ�
				if (_state != ROLL)
					_speed = 7;
				if (isChangableToIdle())	// �ٴ»��°� �ƴϰ� ���ݻ��°� �ƴϸ� IDLE
				{
					_state = IDLE;	// ���� IDLE�̿��� �ϴ��� , �ƴϸ� ���� IDLE�� �ٲ�� �ȵǴ��� �����غ���
				}
				break;
			}
		}
		else if (_state == FLY && upCollision && !(r == 255 && g == 0 && b == 255))
		{
			_x = _oldX;
		}
		else
		{
			if (r == 0 && g == 0 && b == 0)	// ����Ÿ�� �ƴϸ� �˻�
			{
				_y = i - (OTUS_HEIGTH / 2);	// _y�� i�� �˻��� �������� ���ͽ��� ����/2��ŭ �÷��ش�.
				//_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);

				_gravity = 0.f;				// �߷��� 0���� �ٲ��ְ�		
				_axisY = NONE;	// ��� �����ϴ°��� ����
				_FY = FLY_N;	//
				_jumpCount = 0;	// ���� �����ϱ� ����ī��Ʈ 0���� �ٲ��ش�.
				_isFly = false;	// ���� �ʴ� ���¶�°� �ٲ��ֱ�
				if (_state != ROLL)
					_speed = 7;

				if (isChangableToIdle())	// �ٴ»��°� �ƴϰ� ���ݻ��°� �ƴϸ� IDLE
				{
					_state = IDLE;	// ���� IDLE�̿��� �ϴ��� , �ƴϸ� ���� IDLE�� �ٲ�� �ȵǴ��� �����غ���
				}
				break;
			}
		}
	}
	//���� �˻�
	//for (int i = _hitBox.left + _speed; i >= _hitBox.left; i--)
	for (int i = _x - (OTUS_WIDTH/2)+_speed; i>= _x -(OTUS_WIDTH/2); i--)
	{
		color = GetPixel(_mapPixel->getMemDC(), i, _y);	// for���� ���鼭 _x�� i�� �˻� 
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);

		if ((r == 0 && g == 0 && b == 0)) // �������̸� �˻�, �������� �˻�
		{
			_x = i + (OTUS_WIDTH / 2);
			//_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);

			break;
		}
	}
	//������ �˻�
	//for (int i = _hitBox.right - _speed; i <= _hitBox.right; i++)
	for (int i = _x + (OTUS_WIDTH/2)- _speed; i <= _x + (OTUS_WIDTH/2); i++)
	{
		color = GetPixel(_mapPixel->getMemDC(), i, _y);
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);
		if ((r == 0 && g == 0 && b == 0))	// ����Ÿ�� �ƴϸ� �˻� ����Ÿ�� ����
		{
			_x = i - (OTUS_WIDTH / 2);
			//_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);

			break;
		}
	}
}

// �÷��̾�� ������ �浹 ó��
void player::collideActor()
{
	//_enemyManager->getVEnemy()[0]->getHitbox(); // ���ʹ̸Ŵ����� �ִ� 0�� ���ʹ��� ��Ʈ�ڽ� , �Ǵ� ���ʹ��� �������� ������ �ҷ��� �� �ִ�.

	if (_enemyManager != NULL)															// ���ʹ̸Ŵ����� NULL�� �ƴϸ�
	{
		for (int i = 0; i < _enemyManager->getVEnemy().size(); i++)						// �ݺ����� ����
		{
			RECT temp;
			if (_state == ATK)	// ���� ������ ���� �������� �˹���ϴ°� �˻��ϸ�ȵȴ�. ������ �ϰ� �ڷ� �з�����.
			{
				if (IntersectRect(&temp, &_spinHitBox, &_enemyManager->getVEnemy()[i]->getHitbox()))	// �÷��̾��� ���� ��Ʈ�� i��° ���ʹ��� ��Ʈ�ڽ��� �˻��Ѵ�.
				{
					_enemyManager->getVEnemy()[i]->damaged(this);	// ���ʹ̸Ŵ����� i��°�� ���ʹ̿��� �÷��̾��� ������ �Ѱ����
					_isBack = true;
					_backSpeed = 20;
				}
			}
			else if (_state == ROLL)
			{
				if (IntersectRect(&temp, &_rollHitBox, &_enemyManager->getVEnemy()[i]->getHitbox()))
				{
					_enemyManager->getVEnemy()[i]->damaged(this);
					_isBack = true;
					_backSpeed = 20;
					changeState(ATK);
				}
			}
			//HIT���°� �ƴϰ� _state�� �����̾ƴϰ� �����Ⱑ �ƴҶ��� HIT�� �Ǿ�� �Ѵ�. HIT���¿��� ��� ���� �ʰ� HIT�� �ƴҶ��� HIT
			//��ũ�� �÷��̾���� �浹üũ�� �ؼ� �浹�� �Ǹ� _player->damaged(this) �÷��̾�� �ش�. 
			/*
			if (_state != HIT && _state != ATK && _state != ROLL)
			{
				if (IntersectRect(&temp, &_hitBox, &_enemyManager->getVEnemy()[i]->getHitbox()))
				{

				}
			}

			if (_isKnockBack == true)
			{
				_speed = _knockBackSpeed;
				if (_isLeft)
					_x += _speed;
				else
					_x -= _speed;
			}*/
			
		}
	}
	if (_isKnockBack == true)
	{
		_knockBackSpeed -= 0.5f;
		if (_knockBackSpeed < 0)
			_knockBackSpeed = 0;

		_speed = _knockBackSpeed;
		if (_isLeft)
			_x += _speed;
		else
			_x -= _speed;
	}

	if (_isBack == true)
	{
		//���߿� �з����� ��� �߰��ϴ��� �ϱ�
		//changeState(_beforeState);

		_backSpeed -= 1.0f;
		if (_backSpeed < 0)
			_backSpeed = 0;

		_speed = _backSpeed;
		if (_isLeft)
			_x += _speed;
		else
			_x -= _speed;
	}
	
}

void player::collideStuff()
{
	RECT temp;
	if (_liftableActor == NULL)	// _liftableActor�� NULL�̸� �ƹ��͵� �ȵ���ִ� ��
	{
		if (IntersectRect(&temp, &_hitBox, &_geddy->getHitbox()))	// �÷��̾��� ��Ʈ�ڽ��� �Ե��� ��Ʈ�ڽ��� �浹�ϸ�
		{
			_liftableActor = _geddy;								// NULL�̴ϱ� geddy�� ����ִٴ°��� �� �� �ִ�.
		}
		else
		{
			_liftableActor = _stuffManager->collide(this);			//  �÷��̾�� �浹�Ѱ��� ���. �����̸� ���� �����߸� ������ ���
		}
	}
}

//���Ͱ� �÷��̾ ������ �����Ǵ������ŭ �÷��̾��� HP�� ����ش�. �׸��� �÷��̾�� �´� ����� �����ش�.

void player::damaged(actor * e)
{
	if (_state == HIT || _state == ATK || _state == ROLL || _state == DEAD)		// �°ų�, �����ϰų� , ������ �� �Լ��� �����Ѵ�. ��� ����� �����ʰ�
	{
		return;
	}
	//this�� ������ �÷��̾ �ڱ��ڽ��� �����°Ű� ���ʹ� �̹� �÷��̾�� �ڽ��Ǵ������ �Ѱ����.
	//���� HIT�� �ƴҶ��� �������� �ް� ���Ͱ� ����������� �������� �޴´�.
	if (_state != HIT && e->getIsActive() == TRUE && _state != DEAD)
	{
		_hp -= e->getPower();
		UIMANAGER->flickering(RGB(255, 0, 0), 10, 1);	// �ǰݴ����� �� ȭ���� �Ӿ����°�
		changeState(HIT);
		if (_hp <= 0)
		{
			_hp = 0;
			changeState(DEAD);	// hp�� 0�� ���ų� ���ϰ� �Ǹ� 0���� ���������ְ� DEAD���·� �ٲ��ش�.
			UIMANAGER->startingSceneChange(_x, _y);
			_isKnockBack = false;
		}
		_hpBar->setGauge(_hp, _maxHp);
		_isKnockBack = true;
		_knockBackSpeed = 10.f;
		
		if (_liftableActor != NULL && (_state == HIT || _state == DEAD))
		{
			_liftableActor->setState(ON_AIR);
			_liftableActor = NULL;
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

	if (_count % _frameCount[_state] == 0)
	{
		_index++;
		if (_index > img[_state]->getMaxFrameX())
		{
			if (_state == FLY)	// �������� �� �����Ӹƽ����Ǹ� �������Ҹ� �������ϱ�
				SOUNDMANAGER->play("������", _effectVolume / 3);
			if (_state == ATK || _state == ROLL || _state == HIT)
			{
				_isBack = false;

				if (_beforeState == JUMP || _beforeState == JUMPFALL)
				{
					changeState(FLY);
					_gravity = 0.f;
				}
				else
					changeState(_beforeState);
			}
			else if (_state == JUMPFALL)
			{
				_index = img[_state]->getMaxFrameX() - 2;
			}
			else if (_state == DEAD)
			{
				_index = img[_state]->getMaxFrameX();
			}
			else
			{
				_index = 0;
				_isKnockBack = false;
			}
		}
		img[_state]->setFrameX(_index);
	}	
}

int player::sumCoin(int coin)
{
	_coin += coin;


	return _coin;
}
