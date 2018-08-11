#include "stdafx.h"
#include "player.h"
#include "enemyManager.h"		
#include "stuffManager.h"

/*
	 전방선언한 enemyManager클래스는 여기서 가져다 사용해라. include안해주면 enemyMagager에 있는 멤버를 전혀 사용할 수 없다.
	 상호참조할 클래스
*/

/*
	코딩할 때 실행 순서 잘 보기
	조건을 어떻게 걸어줄지 잘 생각해 보기
	걸어준 조건이 잘못되지 않았는지 잘 살펴보기
*/

/*
	마우스 우클릭을 누르고있으면 포물선을 그려주는것
	WALK일 때 대각선으로 내려가는거
	
*/

HRESULT player::init()
{
	//플레이어 상태 (로딩씬에 이미지 들어가있음)
	img[IDLE] = IMAGEMANAGER->findImage("IDLE");
	img[WALK] = IMAGEMANAGER->findImage("Walk");
	img[JUMP] = IMAGEMANAGER->findImage("Jump");
	img[JUMPFALL] = IMAGEMANAGER->findImage("Fall");
	img[FLY] = IMAGEMANAGER->findImage("FLY");
	img[FLYDOWN] = IMAGEMANAGER->findImage("FLYDOWN");
	img[ROLL] = IMAGEMANAGER->findImage("ROLL");
	img[ATK] = IMAGEMANAGER->findImage("ATTACK");
	img[HIT] = IMAGEMANAGER->findImage("DAMAGED");									// 오터스 맞는모션 
	img[DEAD] = IMAGEMANAGER->findImage("DEAD");
	img[LIFT] = IMAGEMANAGER->findImage("LIFT");									// 들어올리는 모션 나오게, 우클릭 상태로 날아다니기 ( 이거는 물체 뒤에서 잡고있는 프레임 )
	for (int i = 0; i < STATE_MAX; ++i)
	{
		_frameCount[i] = 7;
	}
	_frameCount[ROLL] = 5;
	_frameCount[ATK] = 5;
	_frameCount[FLY] = 8;
	_liftImg = IMAGEMANAGER->findImage("LIFT2");									//	(물체 앞에서 잡고있는 프레임)

	//hp바
	friendsFace = IMAGEMANAGER->findImage("FRIEND_UI");
	if (_hpBar == NULL)
	{
		_hpBar = new progressBar;
		_hpBar->init("HP_FRONT", "HP_BACK", 120, 63, 220, 30);
		_maxHp = 30;	// 맥스HP
		_hp = 15;	// 현재 에너지(이미지수정때문에29)
		_hpBar->setGauge(_hp, _maxHp);
	}
	//코인
	_maxCoin = 9999;
	_coin = 0;

	_isDead = false;
	_isLeft = false;																// true = 왼쪽 , false = 오른쪽
	_isFly = false;																	// 날고있는지 아닌지
	_isKnockBack = false;															// 넉백인지 아닌지
	_isBack = false;																// 공격하고 뒤로밀려나기
	_x = 0.f;																		// 플레이어 x좌표
	_y = 0.f;																		// 플레이어 y좌표
	_z = 10;
	_power = 0;																		// 오터스의 파워 초기화
	_speed = 7.0f;																	// 플레이어 달리기 속도 ( 나중에 상태에 따라서 날고있을 때 속도,땅에있을때 속도, 구를때 속도 등등 상태에 따라 속도 바꿔주기 )	
	_jumpSpeed = 15.f;																// 플레이어 점프 속도
	_jumpCount = 0;																	// 점프 카운트
	_flySpeed = 9.f;																// 날기 속도
	_rollSpeed = 15.f;																// 구르기 속도
	_knockBackSpeed = 10.f;															// 뒤로 밀려나는(넉백)속도
	_backSpeed = 20.0f;																// 뒤로 밀려나는 속도
	_angle = PI / 2;																// 플레이어 각도
	_gravity = 0;																	// 플레이어 중력
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);						//오터스의 몸통히트박스
	_spinHitBox = RectMakeCenter(_x, _y, OTUS_WIDTH * 4, OTUS_HEIGTH / 3);			// 오터스의 왼쪽공격(회전) 히트박스 
	_rollHitBox = RectMakeCenter(_x, _y, OTUS_WIDTH * 3, OTUS_HEIGTH * 1.2);		//오터스의 구르기 히트박스

	_count = _index = 0;															// 프레임이미지 돌리기 위한 초기화

	_state = IDLE;																	// 기본 상태 = 가만히있는 상태 (플레이어상태)
	_axisX = _axisY = NONE;															// 키가 안눌린 상태 (키 상태)
	_FX = _FY = FLY_N;																// 날고있을 때 키 상태 초기화 NONE
	_beforeState = _state;															// 이전 상태에 현재 상태를 저장
	_oldX = 0;																		// 이전 X위치
	_oldY = 0;																		// 이전 Y위치
	_walkY = 0;

	_enemyManager = NULL;															// townScene에 들어갔을 땐 0으로 만들어준다. 초기화해준다. (안해주면 쓰레기값을 가져와서 터짐)
	_liftableActor = NULL;															// 초기화 ( 안해주면 터짐 )

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
	if (_state != HIT && _state != DEAD)																// 넉백상태가 아닐 때 (충돌이 아닐 때)
	{
		if (_isFly == true)															// _isFly가 true이면, 날고있는 상태면
		{
			if (_state == ROLL)														// 날고있는 중에 구르기 상태가 되면
				_speed = _rollSpeed;												// 스피드 = 구르기 스피드로
			else
				_speed = 10.f;														// 그렇지 않으면 날고있을 때 속도는 10이다.

			_axisY = NONE;															// 땅에서 _isFly로 하면 _axisY가 점프인 상태로 고정돼서 NONE으로 바꿔준다.
			_gravity = 0;															// 떨어지지 않게 0으로 고정시켜줌
			flyInputKey();															// 날고있을 때 키 입력
			flyAngle(_FX, _FY);														// 입력받은 키에 따라 각도 설정해주기
			flyMove();																// 설정된 각도에 따라 움직인다.
		}

		else																		// 땅에 있을 때
		{
			_speed = 7.0f;															// 땅에 있을 때 스피드 7.0f
			groundInputKey();														// 땅에 있을 때 입력하는 키
			groundAxis(_axisX, _axisY);												// 상태에 따라 움직여주기
		}

		commonInputKey();

		if (_liftableActor != NULL)
		{
			changeState(LIFT);
			_liftableActor->lifted(this);
		}
	}
	// 아래 상하좌우 검사하기전에 히트박스를 먼저 갱신 해주고 검사를한다.
	// 오투스의 렉트 
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);	
	_spinHitBox = RectMakeCenter(_x, _y, OTUS_WIDTH * 4, OTUS_HEIGTH / 3);			 // 오터스의 왼쪽공격(회전) 히트박스 
	_rollHitBox = RectMakeCenter(_x, _y, OTUS_WIDTH * 3, OTUS_HEIGTH * 1.2);
	//충돌함수
	this->collide();
	//체력바 업데이트
	_hpBar->update();
	_hpBar->setGauge(_hp, _maxHp);
	//아울보이 프레임 돌리기
	this->frameSetting();
}

void player::render()
{
	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		Rectangle(getMemDC(), _rollHitBox.left - CAM->getX(), _rollHitBox.top - CAM->getY(), _rollHitBox.right - CAM->getX(), _rollHitBox.bottom - CAM->getY());
		Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());		
	}
	if (_state == ATK)	// 렉트가 공격할때만 나타나게
	{
		//Rectangle(getMemDC(), _spinHitBox.left - CAM->getX(), _spinHitBox.top - CAM->getY(), _spinHitBox.right - CAM->getX(), _spinHitBox.bottom - CAM->getY());
	}
	img[_state]->frameRender(getMemDC(), _x - CAM->getX() - img[_state]->getFrameWidth() / 2, _y - CAM->getY() - img[_state]->getFrameHeight() / 2);

	char str[128];
	//sprintf_s(str, "X좌표 : %.f  Y좌표 : %.f  중력 : %.f 인덱스 : %d  스피드 : %.f   각도 : %.2f   x축 : %d   y축 : %d 상태 : %d  점프카운트 : %d", _x, _y,_gravity, _index, _speed, _angle, _FX, _FY,_state, _jumpCount);
	sprintf_s(str, " 코인 : %d", _coin);
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

	//천의자리 1234 / 1000 = 1.234니까 1이 나옴
	IMAGEMANAGER->findImage("number")->setFrameX(_coin / 1000);
	IMAGEMANAGER->findImage("number")->frameRender(getMemDC(), 50, 150);
	
	//백의 자리 
	IMAGEMANAGER->findImage("number")->setFrameX(((_coin % 1000)/100));
	IMAGEMANAGER->findImage("number")->frameRender(getMemDC(), 70, 150);

	//십의자리
	IMAGEMANAGER->findImage("number")->setFrameX((_coin % 100)/10);
	IMAGEMANAGER->findImage("number")->frameRender(getMemDC(), 90, 150);
	
	//일의자리
	IMAGEMANAGER->findImage("number")->setFrameX(_coin % 10);
	IMAGEMANAGER->findImage("number")->frameRender(getMemDC(), 110, 150);

	//(_coin  % 100) /10
	//	(_coin % 10)
	/*
	1234 / 1000 == 1.234 >> 1이 나오고
	(1234 % 1000) / 100 >> 2가 나오고
	1234 / 100 == 12.34
	1234 % 100 == 34
	1280 / 100 == 1.280
	1280 % 100 == 280
	(1280 % 100) / 10 28
	*/
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//키입력함수 ( 불값, 상태정도만 바꿔주기 )
void player::groundInputKey()
{
	_axisX = NONE;																	// 키 입력이 없으면 _axisX 는 NONE이 된다.
	if (_state == JUMPFALL)															// 떨어지고 있을 때 W를 누르면 바로 날고있는 상태가 될 수 있게 _jumpCount를 1로 해준다.
		_jumpCount = 1;

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE) && _state != ROLL && _liftableActor == NULL)
	{
		changeState(ROLL);
		if (_state == ROLL)
		{			
			if (_isLeft)
				EFFECTMANAGER->play("구르기왼쪽", _x, _y);							// 내가 왼쪽으로 구르면 왼쪽 이펙트
			else
				EFFECTMANAGER->play("구르기오른쪽", _x, _y);							// 내가 왼쪽으로 구르면 왼쪽 이펙트		

			SOUNDMANAGER->play("구르기", _soundVolume);
		}
		_rollHitBox = RectMakeCenter(_x, _y, OTUS_WIDTH * 3, OTUS_HEIGTH * 1.2);
	}
	if (KEYMANAGER->isStayKeyDown('A'))	
	{
		_dir = LEFT;
		_isLeft = true;																// 프레임 방향 구분 true면 왼쪽 프레임을 보여준다.
		_axisX = LEFT;																// LEFT 왼쪽으로 이동
		if (_state == ATK)
			changeState(ATK);
		else if (_state == ROLL)
		{
			changeState(ROLL);
		}
		else if (_state != JUMP && _state != JUMPFALL && _state != ROLL)			// 점프나 떨어지는 상태일 때 뛰는모션 안나오게 조건울 걸어준다.
			changeState(WALK);														// 스테이트가 바뀔때 마다 index, count 0으로초기화해주는 함수.
	}
	else if (KEYMANAGER->isStayKeyDown('D'))
	{
		_dir = RIGHT;
		_isLeft = false;															// 프레임 방향 구분 false면 오른쪽 프레임을 보여준다.
		_axisX = RIGHT;																// RIGHT면 오른쪽으로 이동
		if (_state == ATK)
			changeState(ATK);
		else if (_state != JUMP && _state != JUMPFALL && _state != ROLL)			// 점프나 떨어지는 상태일 때 뛰는모션이 안나오게 조건을 걸어준다.
			changeState(WALK);														// 스테이트가 바뀔때 마다 index, count 0으로초기화해주는 함수.
		
	}
	if (_state != JUMP )															// 키를 안누르면 바로 NONE이 돼서 점프가 안됨
		_axisY = NONE;
	if (KEYMANAGER->isOnceKeyDown('W'))
	{
		if (_state == ATK)
			changeState(ATK);	
		else		
			changeState(JUMP);														// 위로 점프하는 프레임을 보여준다.
		
		_axisY = UP;																// UP이 되면 위로 점프를 한다.
		_jumpCount += 1;															// _jumpCount가 2가 되면 FLY상태로 바꿔준다.
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
		_gravity -= 0.16f;															// W키를 오래 누르고 있을 수록 높이 점프할 수 있다.
	}
	
}

// 복습 할 부분
void player::groundAxis(WAY axisX, WAY axisY)										// 키 입력으로 바꿔준 상태나 불값을 따라서 움직여주기
{
	//위아래가 동시에 충돌했을때  , _oldX = _x 이전위치x에 현재위치 X를 대입하고 
	_oldX = _x;
	_oldY = _y;									// 이전 위치(_oldY)에 현재위치(_y)를 저장한다.
	if (_axisX == NONE && _axisY == NONE && _state != ATK && _state != ROLL && _state != JUMPFALL && _state != LIFT && _state != HIT && _state != DEAD )		// X,Y 키가 둘 다 NONE이면 _state = IDLE 상태이다.
		_state = IDLE;

	/*
	_y가 0이 들어오게하기. 순서가 중요하다.
	_gravity += 0.5f가 위에 있으면 땅에 닿아도 0이아니라 0.5f이 더해져서 JUMPFALL상태가 돼서 아래로 내려간다.
	_y가 위에 있으면 계속 0이 더해진다.
	*/
	if (_state != ROLL)
	{
		if (_gravity > _jumpSpeed * 2)
			_gravity = _jumpSpeed * 2;
		_y += _gravity;		// y에 중력값을 계속 더해준다. 
		_gravity += 0.5f;	// 아래로 내려갈 수 있게 점프가 아니어도 계속 중력값을 더해준다.
		if (_state == ATK)
		{
			_gravity -= 0.3f;
		}
	}
	if (_axisY == UP)	// _axisY == UP일 때 위로 점프를 시켜준다.	
		_y += -sinf(PI / 2) * _jumpSpeed;
	
	/* 
	-sinf(angle) * speed << 음수값
	_gravity += -sinf(angle) * speed; << 이거를 계속 더하면 아래로 떨어지는게 아니라 오히려 점프를 함
	그래서 한번만 실행하게 조건을 걸어줘야 함
	*/

	//떨어지는 상황 : 중력이 속도보다 커지기, 현재위치가 이전위치보다 클때
	//반드시 공부하기 중요 
	if (_y > _oldY)
	{
		//공격중에 바로 jumpfall이 안되게
		//if (_state != ROLL && _state != ATK && _state != WALK)
		if (_state != ROLL && _state != ATK)
		{
			// 한번만 실행시켜줘야한다.
			// 점프를 했다가 떨어질때 자연스럽게 떨어지기 위해 중력을 조절
			if (_state == JUMP)
				_gravity += -sinf(PI / 2) * _jumpSpeed;
			changeState(JUMPFALL);			// 스테이트가 바뀔때 마다 index, count 0으로초기화해주는 함수.
		}
	}

	if (_state == ROLL)				// 구르기 상태일 때
	{
		_speed = _rollSpeed;		// 스피드는 구르는스피드
		if (_isLeft)				// 왼쪽일 때
		{
			_x -= _speed;			// 왼쪽으로 스피드만큼
		}
		else if (!_isLeft)			// 오른쪽으로 스피드만큼
		{
			_x += _speed;
		}
	}
	else																					// 구르는 상태가 아니라면 스피드는 7이고 구르는속도랑 걷기속도가 누적되지않게 해준다.
	{
		_speed = 7.0f;					
		if (_axisX == LEFT)																	//  _axisX == LEFT일 때 왼쪽으로 움직여준다.
		{
			_x -= _speed;
		}
		else if (_axisX == RIGHT)															//_axisX == RIGHT일 때 오른쪽으로 움직여준다.
		{
			_x += _speed;
		}
	}
}

//날고 있을 때
void player::flyInputKey()
{
	if (_state != ROLL)																		// 구르기 상태가 아닐 때는 _FX를 NONE으로 해준다. 
		_FX = FLY_N;			

	if (_FX == NONE && _FY == NONE && _state != ATK && _state != ROLL && _state != LIFT)	//_FX,_FY가 NONE이고 _state가 공격,구르기,들고있는 상태가 아닐 때 FLY
		changeState(FLY);																	// 스테이트가 바뀔때 마다 index, count 0으로초기화해주는 함수.

	
	if (KEYMANAGER->isStayKeyDown('A'))														// 대쉬 한 후에 느려지고 날라가는거 유지
	{
		_isLeft = true;																		// _isLeft = true면 왼쪽 프레임을 보여준다.
		_FX = FLY_L;																		// _FX 가 왼쪽이 눌렸음을 알려준다.
		if (_state == ATK)																	// A를 눌렀는데 공격상태면 공격 모션을 보여주고 ROLL이면 ROLL, LIFT면 LIFT 그렇지 않으면 FLY
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
			changeState(FLY);																// 스테이트가 바뀔때 마다 index, count 0으로초기화해주는 함수.
	}
	if(_state != ROLL)																		// 구르는 상태가 아닐때 _FY를 NONE으로 바꿔준다.
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
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE) && _state != ROLL)								// 구르는 도중에 다시구르기 안하게 걸어준다.
	{
		changeState(ROLL);
		SOUNDMANAGER->play("구르기", _effectVolume);
		if (_FY == NONE && _liftableActor == NULL)																	// 위아래로 날때는 구르는이펙트가 안나오게 해준다.
		{			
			if (_isLeft)
				EFFECTMANAGER->play("구르기왼쪽", _x, _y);									// 내가 왼쪽으로 구르면 왼쪽 이펙트
			else
				EFFECTMANAGER->play("구르기오른쪽", _x, _y);									// 내가 왼쪽으로 구르면 왼쪽 이펙트		
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

	if (_FX == FLY_L)	// 왼쪽
	{
		if (_FY == FLY_U)
			fAngle = 135.f;
		else if (_FY == FLY_D)
			fAngle = 225.f;
		else
			fAngle = 180.f;
	}
	else if (_FX == FLY_R)	// 오른쪽 
	{
		if (_FY == FLY_U)
			fAngle = 45.f;
		else if (_FY == FLY_D)
			fAngle = 315.f;
		else
			fAngle = 0.f;
	}
	else if (_FX == NONE)											// NONE을 안걸어 주면 키를 두개 눌렀을 때 대각선으로 안가고 위나 아래로 움직인다.
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
	_oldX = _x;														// _oldX(이전위치)에 _x(현재위치)를 대입한다. 
	if (_state == ROLL)												// 구르기 상태면
	{
		_flySpeed = _rollSpeed;										// 구르기 상태일 때는 flySpeed가 rollSpeed가 되고

		_x += cosf(_angle) * _flySpeed;
		_y += -sinf(_angle) * _flySpeed;
	}
	else
	{
		_flySpeed = 10.f;
		if (_FX != NONE)											// 안해주면 계속 움직이니까 _FX키를 눌렀을 때만 움직이게 해준다.
			_x += cosf(_angle) * _flySpeed;
		_y += -sinf(_angle) * _flySpeed;
	}
}

//공부하기
void player::changeState(int state)									// 상태(스테이트)가 바뀔때 마다 index, count 0으로초기화해주는 함수.
{
	if (_state == state) return;									// 매개변수는 함수가 끝나면 없어진다.	

	if (state != ATK && state != ROLL)								// 날고있을 때 공격이나 구르기를 하면 _isFly가 false처리가 안되게
	{
		if (state == FLY || state == FLYDOWN || state == LIFT)		// state가 FLY이거나 FLYDOWN이거나 LIFT일때 _isFly가 false 안되게 조건을 걸어준다.
			_isFly = true;
		else														// state가 FLY가 아니고 FLYDOWN이 아니고 LIFT가 아니면 false
			_isFly = false;
	}
	// _state가 공격일 때 _beforeState에 공격이 들어가고 state에 구르기일때 _state에 구르기가 들어가서 _beforState가 공격,구르기를 반복하니까 방지해줘야한다.
	// 공격 구르기가 아닐때만 이전상태를 저장하면 반복하지 않는다.
	if (_state != ATK && _state != ROLL && _state != LIFT && _state != DEAD)
	{
		_beforeState = _state;										// 구르기->공격을 하면 아무것도 입력 안했는데 구르기공격만 반복하기 때문에 걸어준 조건. 
	}
	_state = state;

	_index = 0;
	_count = 0;
}

//공부하기
void player::commonInputKey()														 // 날고있을때나 땅에 있을 때 공용 키 입력
{																					
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON) && _state != ATK)
	{							
		SOUNDMANAGER->play("공격", _effectVolume);
		if (_liftableActor == NULL)													// _liftableActor가 NULL이면 오터스가 아무것도 안들고 있는 상태이기때문에 오터스가 공격한다.											
		{																			
			changeState(ATK);														 // 코드를 한줄로 묶을때 함수로 만드는데 달라지는 변수를 매개변수로 빼준다.	
			_gravity = 0.f;															 // 땅에 있을 때나 날고 있을 때 중력은 어차피 0
			_spinHitBox = RectMakeCenter(_x, _y, OTUS_WIDTH * 4, OTUS_HEIGTH / 3);	 // 오터스의 왼쪽공격(회전) 히트박스 
		}
		else
		{
			_liftableActor->attack();												 // _liftableActor가NULL이 아니면 게디가? 들고있는 대상이? 공격한다.
			_liftableActor = _liftableActor->use(this);
			changeState(FLY);
		}
	}
	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		if (_liftableActor != NULL)													// _liftableActor가 NULL이 아니면 오터스가 뭔가 들고있는 상태이다.
		{
			//내 마우스와 오투스사이의 각도를 구해서 던지고 손에 아무것도 없다는것을 알리기위해 NULL로 바꿔주고 상태도 FLY로 바꿔준다.
			SOUNDMANAGER->play("던지기", _effectVolume);	
			_liftableActor->throwed(10, getAnglef(_x - CAM->getX(),_y - CAM->getY() , _ptMouse.x, _ptMouse.y));

			_liftableActor = NULL;
			changeState(FLY);
		}
		else												// NULL이면
		{
			collideStuff();
			if (_liftableActor != NULL)
			{
				changeState(LIFT);
				if (_liftableActor == _geddy)	// 게디일때만 소환사운드, 들기 이펙트 
				{
					SOUNDMANAGER->play("소환", _effectVolume);
					EFFECTMANAGER->play("들기", _x, _y);
				}				

				_liftableActor->lifted(this);
			}
		}
	}
	else if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))	// 우클릭을 누르고있을 때 포물선 그려주기
	{
		if (_liftableActor != NULL)
		{

		}
	}
	if (KEYMANAGER->isOnceKeyDown('Q'))	// 게디 소환
	{
		if (_liftableActor == NULL ) 
		{
			SOUNDMANAGER->play("소환", _effectVolume);
			EFFECTMANAGER->play("소환", _x, _y);
			EFFECTMANAGER->play("소환", _geddy->getX(), _geddy->getY());
			_liftableActor = _geddy;								// NULL이니까 geddy를 들고있다는것을 알 수 있다.
			_liftableActor->setIsActive(TRUE);
		}			
	}
}

//충돌함수
void player::collide()
{
	//충돌처리함수 ( 맵 , 몬스터랑 플레이어 )
	this->collideMap();
	this->collideActor();
}

void player::collideMap()
{
	bool upCollision = false;	// 위아래가 충돌했는데 x가 안으로 들어가서 버그나지않게 이전위치로 이동시켜준다
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

	//위에 검사
	//for (int i = _hitBox.top + _speed; i >= _hitBox.top; i--)
	for (int i = _y - (OTUS_HEIGTH / 2) + _speed; i >= _y - (OTUS_HEIGTH / 2); i--)
	{
		color = GetPixel(_mapPixel->getMemDC(), _x, i);
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);

		if ((r == 0 && g == 0 && b == 0)) // 검은색만 검사
		{
			if (!_isFly)
				_x = _oldX;
			_y = i + (OTUS_HEIGTH / 2);
			//_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);
			upCollision = true;
			break;
		}
	}
	//아래 검사
	//for (int i = _hitBox.bottom - _speed; i <= _hitBox.bottom; i++)
	//walk상태 일때만 조금 더 아래를 검사한다.
	if (_state == WALK)
		_walkY = 20;
	else
		_walkY = 0;

	for (int i = _y + (OTUS_HEIGTH /2) - _speed; i <= _y + (OTUS_HEIGTH / 2) + _walkY; i++)
	{
		color = GetPixel(_mapPixel->getMemDC(), _x, i);	// for문을 돌면서 _y를 i로 검사
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);

		if (_state == JUMPFALL || _state == FLYDOWN || _state == WALK || _state == ROLL || _state == ATK || _state == IDLE )	// 아래에서 위로 올라갈때 검사하면 안되니까 isFly가 false이거나 아래로 내려올때만 검사 한다.
		{
			if (!(r == 255 && g == 0 && b == 255))	// 마젠타가 아니면 검사
			{
				_y = i - (OTUS_HEIGTH / 2);	// _y는 i로 검사한 지점부터 오터스의 세로/2만큼 올려준다.
				//_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);

				_gravity = 0.f;				// 중력을 0으로 바꿔주고		
				_axisY = NONE;	// 계속 점프하는것을 방지
				_FY = FLY_N;	//
				_jumpCount = 0;	// 땅에 있으니까 점프카운트 0으로 바꿔준다.
				_isFly = false;	// 날지 않는 상태라는걸 바꿔주기
				if (_state != ROLL)
					_speed = 7;
				if (isChangableToIdle())	// 뛰는상태가 아니고 공격상태가 아니면 IDLE
				{
					_state = IDLE;	// 언제 IDLE이여야 하는지 , 아니면 언제 IDLE로 바뀌면 안되는지 생각해보기
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
			if (r == 0 && g == 0 && b == 0)	// 마젠타가 아니면 검사
			{
				_y = i - (OTUS_HEIGTH / 2);	// _y는 i로 검사한 지점부터 오터스의 세로/2만큼 올려준다.
				//_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);

				_gravity = 0.f;				// 중력을 0으로 바꿔주고		
				_axisY = NONE;	// 계속 점프하는것을 방지
				_FY = FLY_N;	//
				_jumpCount = 0;	// 땅에 있으니까 점프카운트 0으로 바꿔준다.
				_isFly = false;	// 날지 않는 상태라는걸 바꿔주기
				if (_state != ROLL)
					_speed = 7;

				if (isChangableToIdle())	// 뛰는상태가 아니고 공격상태가 아니면 IDLE
				{
					_state = IDLE;	// 언제 IDLE이여야 하는지 , 아니면 언제 IDLE로 바뀌면 안되는지 생각해보기
				}
				break;
			}
		}
	}
	//왼쪽 검사
	//for (int i = _hitBox.left + _speed; i >= _hitBox.left; i--)
	for (int i = _x - (OTUS_WIDTH/2)+_speed; i>= _x -(OTUS_WIDTH/2); i--)
	{
		color = GetPixel(_mapPixel->getMemDC(), i, _y);	// for문을 돌면서 _x를 i로 검사 
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);

		if ((r == 0 && g == 0 && b == 0)) // 검은색이면 검사, 검은색만 검사
		{
			_x = i + (OTUS_WIDTH / 2);
			//_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);

			break;
		}
	}
	//오른쪽 검사
	//for (int i = _hitBox.right - _speed; i <= _hitBox.right; i++)
	for (int i = _x + (OTUS_WIDTH/2)- _speed; i <= _x + (OTUS_WIDTH/2); i++)
	{
		color = GetPixel(_mapPixel->getMemDC(), i, _y);
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);
		if ((r == 0 && g == 0 && b == 0))	// 마젠타가 아니면 검사 마젠타를 무시
		{
			_x = i - (OTUS_WIDTH / 2);
			//_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);

			break;
		}
	}
}

// 플레이어와 몬스터의 충돌 처리
void player::collideActor()
{
	//_enemyManager->getVEnemy()[0]->getHitbox(); // 에너미매니져에 있는 0번 에너미의 히트박스 , 또는 에너미의 여러가지 정보를 불러올 수 있다.

	if (_enemyManager != NULL)															// 에너미매니져가 NULL이 아니면
	{
		for (int i = 0; i < _enemyManager->getVEnemy().size(); i++)						// 반복문을 돌고
		{
			RECT temp;
			if (_state == ATK)	// 공격 상태일 때는 오투스가 넉백당하는걸 검사하면안된다. 공격을 하고 뒤로 밀려난다.
			{
				if (IntersectRect(&temp, &_spinHitBox, &_enemyManager->getVEnemy()[i]->getHitbox()))	// 플레이어의 공격 렉트와 i번째 에너미의 히트박스를 검사한다.
				{
					_enemyManager->getVEnemy()[i]->damaged(this);	// 에너미매니져야 i번째의 에너미에게 플레이어의 정보를 넘겨줘라
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
			//HIT상태가 아니고 _state가 공격이아니고 구르기가 아닐때만 HIT가 되어야 한다. HIT상태에서 계속 맞지 않게 HIT가 아닐때만 HIT
			//고크가 플레이어와의 충돌체크를 해서 충돌이 되면 _player->damaged(this) 플레이어에게 준다. 
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
		//나중에 밀려나는 모션 추가하던가 하기
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
	if (_liftableActor == NULL)	// _liftableActor가 NULL이면 아무것도 안들고있는 것
	{
		if (IntersectRect(&temp, &_hitBox, &_geddy->getHitbox()))	// 플레이어의 히트박스와 게디의 히트박스가 충돌하면
		{
			_liftableActor = _geddy;								// NULL이니까 geddy를 들고있다는것을 알 수 있다.
		}
		else
		{
			_liftableActor = _stuffManager->collide(this);			//  플레이어와 충돌한것을 들기. 과일이면 과일 무게추면 무게추 등등
		}
	}
}

//몬스터가 플레이어를 때려서 몬스터의대미지만큼 플레이어의 HP를 깎아준다. 그리고 플레이어는 맞는 모션을 보여준다.

void player::damaged(actor * e)
{
	if (_state == HIT || _state == ATK || _state == ROLL || _state == DEAD)		// 맞거나, 공격하거나 , 죽으면 이 함수를 종료한다. 계속 대미지 입지않게
	{
		return;
	}
	//this를 넣으면 플레이어가 자기자신을 때리는거고 몬스터는 이미 플레이어에게 자신의대미지를 넘겨줬다.
	//내가 HIT가 아닐때만 데미지를 받고 몬스터가 살아있을때만 데미지를 받는다.
	if (_state != HIT && e->getIsActive() == TRUE && _state != DEAD)
	{
		_hp -= e->getPower();
		UIMANAGER->flickering(RGB(255, 0, 0), 10, 1);	// 피격당했을 때 화면이 붉어지는것
		changeState(HIT);
		if (_hp <= 0)
		{
			_hp = 0;
			changeState(DEAD);	// hp가 0과 같거나 이하가 되면 0으로 고정시켜주고 DEAD상태로 바꿔준다.
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

//이미지프레임셋팅
void player::frameSetting()
{
	//잘 살펴보기 (공부하기 )
	_count++;
	if (_isLeft)
		img[_state]->setFrameY(1);
	else if (!_isLeft)		// 오른쪽
		img[_state]->setFrameY(0);

	if (_count % _frameCount[_state] == 0)
	{
		_index++;
		if (_index > img[_state]->getMaxFrameX())
		{
			if (_state == FLY)	// 날고있을 때 프레임맥스가되면 날개짓소리 나오게하기
				SOUNDMANAGER->play("날개짓", _effectVolume / 3);
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
