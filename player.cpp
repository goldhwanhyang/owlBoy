#include "stdafx.h"
#include "player.h"

/*
	코딩할 때 실행 순서 잘 보기
	조건을 어떻게 걸어줄지 잘 생각해 보기
	걸어준 조건이 잘못되지 않았는지 잘 살펴보기
*/

//내려가는 상태인데 허공에서 날라다니는 문제

HRESULT player::init()
{
	//로딩씬에 이미지 들어가있음
	img[IDLE] = IMAGEMANAGER->findImage("IDLE");
	img[WALK] = IMAGEMANAGER->findImage("Walk");
	img[JUMP] = IMAGEMANAGER->findImage("Jump");
	img[JUMPFALL] = IMAGEMANAGER->findImage("Fall");
	img[FLY] = IMAGEMANAGER->findImage("FLY");
	img[FLYDOWN] = IMAGEMANAGER->findImage("FLYDOWN");
	img[ROLL] = IMAGEMANAGER->findImage("ROLL");
	img[ATK] = IMAGEMANAGER->findImage("ATTACK");
	img[LIFT] = IMAGEMANAGER->findImage("LIFT");	// 들어올리는 모션 나오게, 우클릭 상태로 날아다니기 ( 이거는 물체 뒤에서 잡고있는 프레임 )
	img[LIFT2] = IMAGEMANAGER->findImage("LIFT2");	//	(물체 앞에서 잡고있는 프레임)


	_isLeft = false;			// true = 왼쪽 , false = 오른쪽
	_isFly = false;				// 날고있는지 아닌지
	_x = 440.f;					// 플레이어 x좌표
	_y = 810.f;					// 플레이어 y좌표
	_speed = 7.0f;				// 플레이어 달리기 속도 ( 나중에 상태에 따라서 날고있을 때 속도,땅에있을때 속도, 구를때 속도 등등 상태에 따라 속도 바꿔주기 )
	_jumpSpeed = 15.f;			// 플레이어 점프 속도
	_jumpCount = 0;
	_flySpeed = 9.f;
	_rollSpeed = 15.f;
	_angle = PI / 2;			// 플레이어 각도
	_gravity = 0;				// 플레이어 중력
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH); 
	_count = _index = 0;		// 프레임이미지 돌리기 위한 초기화

	_state = IDLE;				// 기본 상태 = 가만히있는 상태 (플레이어상태)
	_axisX = _axisY = NONE;		// 키가 안눌린 상태 (키 상태)
	_FX = _FY = FLY_N;			// 날고있을 때 키 상태 초기화 NONE
	_beforeState = _state;
	_oldY = 0;					// 이전 위치

	return S_OK;
}


void player::release()
{


}


void player::update()
{
	//날고 있을 때
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
	// 땅에 있을 때
	else
	{
		_speed = 7.0f;													// 땅에 있을 때 스피드 7.0f
		groundInputKey();												// 땅에 있을 때 입력하는 키
		groundAxis(_axisX, _axisY);
	}
	// 아래 상하좌우 검사하기전에 히트박스를 먼저 갱신 해주고 검사를한다.
	// 오투스의 렉트 
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);	

	//충돌처리함수
	this->collide();
	//아울보이 프레임 돌리기
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
	sprintf_s(str, "X좌표 : %.f  Y좌표 : %.f  중력 : %.f 인덱스 : %d  스피드 : %.f   각도 : %.2f   x축 : %d   y축 : %d 상태 : %d  점프카운트 : %d", _x, _y,_gravity, _index, _speed, _angle, _FX, _FY,_state, _jumpCount);
	TextOut(getMemDC(), 10, 10, str, strlen(str));
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//키입력함수 ( 불값, 상태정도만 바꿔주기 )
void player::groundInputKey()
{
	_axisX = NONE;										// 키 입력이 없으면 _axisX 는 NONE이 된다.
	if (_state == JUMPFALL)								// 떨어지고 있을 때 W를 누르면 바로 날고있는 상태가 될 수 있게 _jumpCount를 1로 해주고
		_jumpCount = 1;
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON) && _state != ATK)
	{
		EFFECTMANAGER->play("구르기왼쪽", _x, _y);	// 내가 왼쪽으로 구르면 왼쪽 이펙트

		changeState(ATK); // 코드를 한줄로 묶을때 함수로 만드는데 달라지는 변수를 매개변수로 빼준다.	
	}
	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		changeState(LIFT);	// 우클릭을 하면 들어올리는 모션이 나와야한다.
		_isFly = true;		// 날고있는 상태로 바뀜
	}
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE) && _state != ROLL && _state != JUMP)
	{
		EFFECTMANAGER->play("구르기왼쪽", _x, _y);	// 내가 왼쪽으로 구르면 왼쪽 이펙트
		changeState(ROLL);
	}
	if (KEYMANAGER->isStayKeyDown('A'))	
	{
		_dir = LEFT;
		_isLeft = true;									// 프레임 방향 구분 true면 왼쪽 프레임을 보여준다.
		_axisX = LEFT;									// LEFT 왼쪽으로 이동
		if (_state == ATK)
			changeState(ATK);

		else if (_state == ROLL)
		{
			changeState(ROLL);
		}
		else if (_state != JUMP && _state != JUMPFALL && _state != ROLL)		// 점프나 떨어지는 상태일 때 뛰는모션 안나오게 조건울 걸어준다.
			changeState(WALK);							// 스테이트가 바뀔때 마다 index, count 0으로초기화해주는 함수.
	}
	else if (KEYMANAGER->isStayKeyDown('D'))
	{
		_dir = RIGHT;
		_isLeft = false;								// 프레임 방향 구분 false면 오른쪽 프레임을 보여준다.
		_axisX = RIGHT;									// RIGHT면 오른쪽으로 이동
		if (_state == ATK)
			changeState(ATK);
		else if (_state != JUMP && _state != JUMPFALL && _state != ROLL)		// 점프나 떨어지는 상태일 때 뛰는모션이 안나오게 조건을 걸어준다.
			changeState(WALK);							// 스테이트가 바뀔때 마다 index, count 0으로초기화해주는 함수.
		
	}
	if (_state != JUMP )	// 키를 안누르면 바로 NONE이 돼서 점프가 안됨
		_axisY = NONE;
	if (KEYMANAGER->isOnceKeyDown('W'))
	{
		_axisY = UP;									// UP이 되면 위로 점프를 한다.
		changeState(JUMP);								// 위로 점프하는 프레임을 보여준다.
		_jumpCount += 1;								// _jumpCount가 2가 되면 FLY상태로 바꿔준다.
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
		_gravity -= 0.16f;								// W키를 오래 누르고 있을 수록 높이 점프할 수 있다.
	}
	//_click = NB;	// 
}

// 복습 할 부분
void player::groundAxis(WAY axisX, WAY axisY)	// 키 입력으로 바꿔준 상태나 불값을 따라서 움직여주기
{
	_oldY = _y;									// 이전 위치(_oldY)에 현재위치(_y)를 저장한다.
	if (_axisX == NONE && _axisY == NONE && _state != ATK && _state != ROLL && _state != JUMPFALL && _state != LIFT && _state != LIFT2)		// X,Y 키가 둘 다 NONE이면 _state = IDLE 상태이다.
		_state = IDLE;

	/*
	_y가 0이 들어오게하기. 순서가 중요하다.
	_gravity += 0.5f가 위에 있으면 땅에 닿아도 0이아니라 0.5f이 더해져서 JUMPFALL상태가 돼서 아래로 내려간다.
	_y가 위에 있으면 계속 0이 더해진다.
	*/
	_y += _gravity;		// y에 중력값을 계속 더해준다. 
	_gravity += 0.5f;	// 아래로 내려갈 수 있게 점프가 아니어도 계속 중력값을 더해준다.

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
		// 한번만 실행시켜줘야한다.
		// 점프를 했다가 떨어질때 자연스럽게 떨어지기 위해 중력을 조절
		if(_state == JUMP)
			_gravity += -sinf(PI / 2) * _jumpSpeed;	

		changeState(JUMPFALL);			// 스테이트가 바뀔때 마다 index, count 0으로초기화해주는 함수.
	}

	if (_state == ROLL)				// 구르기 상태일 때
	{
		_speed = _rollSpeed;		// 스피드는 구르는스피드
		if (_isLeft)				// 왼쪽일 때
			_x -= _speed;			// 왼쪽으로 스피드만큼
		else if (!_isLeft)			// 오른쪽으로 스피드만큼
			_x += _speed;
	}
	else		// 구르는 상태가 아니라면 스피드는 7이고 구르는속도랑 걷기속도가 누적되지않게 해준다.
	{
		_speed = 7.0f;				

		if (_axisX == LEFT)		//  _axisX == LEFT일 때 왼쪽으로 움직여준다.
		{
			_x -= _speed;
		}
		else if (_axisX == RIGHT)	//_axisX == RIGHT일 때 오른쪽으로 움직여준다.
		{
			_x += _speed;
		}

	}
	
}

//날고 있을 때
void player::flyInputKey()
{
	if (_state != ROLL)
		_FX = FLY_N;

	if (_FX == NONE && _FY == NONE && _state != ATK && _state != ROLL && _state != LIFT && _state != LIFT2)	// _FX,_FY가 NONE이고 _state가 ATK아니고 ROLL이 아닐 때 FLY상태 
		changeState(FLY);						// 스테이트가 바뀔때 마다 index, count 0으로초기화해주는 함수.

	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		changeState(ATK);
	}
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		changeState(ROLL);
	}
	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		changeState(LIFT);
	}
	else if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		// 마우스를 누르고있으면 오터스가 빙글빙글 돌면서 내려옴 이미지없어서 나중에 추가하면 넣기
	}
	if (KEYMANAGER->isOnceKeyDown('A'))	// 키를 처음 누르면 앞으로 대쉬
	{
		_isLeft = true;
	}
	else if (KEYMANAGER->isStayKeyDown('A'))	// 대쉬 한 후에 느려지고 날라가는거 유지
	{
		_FX = FLY_L;
		if (_state == ATK)
			changeState(ATK);
		else if (_state == ROLL)
			changeState(ROLL);
		else if (_state == LIFT)
			changeState(LIFT);
		else
			changeState(FLY);						// 스테이트가 바뀔때 마다 index, count 0으로초기화해주는 함수.
	}
	if (KEYMANAGER->isOnceKeyDown('D'))	// 키를 처음 누르면 앞으로 대쉬
	{
		_isLeft = false;
	}
	else if (KEYMANAGER->isStayKeyDown('D'))	// 대쉬 한 후에 날라가는거 유지
	{
		_FX = FLY_R;
		if (_state == ATK)
			changeState(ATK);
		else if (_state == ROLL)
			changeState(ROLL);
		else if (_state == LIFT)
			changeState(LIFT);
		else
			changeState(FLY);						// 스테이트가 바뀔때 마다 index, count 0으로초기화해주는 함수.
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
			changeState(FLY);			// 스테이트가 바뀔때 마다 index, count 0으로초기화해주는 함수.
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
			changeState(FLYDOWN);			// 스테이트가 바뀔때 마다 index, count 0으로초기화해주는 함수.
	}
}

float player::flyAngle(FLYING FX, FLYING FY)	
{
	float fAngle = 0;

	//구르고있을때 키를 안눌러도 각도가 유지되게
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
	else if (_FX == NONE)	// NONE을 안걸어 주면 키를 두개 눌렀을 때 대각선으로 안가고 위나 아래로 움직인다.
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
	if (_state == ROLL)							// 구르기 상태일 때는 flySpeed가 rollSpeed가 되고
	{
		_flySpeed = _rollSpeed;

		_x += cosf(_angle) * _flySpeed;
		_y += -sinf(_angle) * _flySpeed;
	}
	else
	{
		_flySpeed = 10.f;
		if (_FX != NONE)	 // 안해주면 계속 움직인다.
			_x += cosf(_angle) * _flySpeed;

		_y += -sinf(_angle) * _flySpeed;
	}
}

//상태가 바뀔때마다 인덱스를 0으로 초기화 
void player::changeState(int state)
{
	if (_state == state) return;	// 매개변수는 함수가 끝나면 없어진다.

	_state = state;
	_index = 0;
	_count = 0;
}


//충돌함수
void player::collide()
{
	COLORREF color;
	int r;
	int g;
	int b;
	//위에 검사

	//for (int i = _speed - 10 ; i < _speed + 10; i++)
	for(int i = _hitBox.top + _speed; i >= _hitBox.top; i--)
	{
		color = GetPixel(_mapPixel->getMemDC(), _x, i);
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);
	
		//isFly일때 isFlyDown이 아닐 때 충돌 검사
		if ((r == 0 && g == 0 && b == 0)) // 검은색만 검사
		{
			_y = i + (OTUS_HEIGTH / 2);
			break;
		}
	}	
	//아래 검사
	for (int i = _hitBox.bottom - _speed; i <= _hitBox.bottom ; i++)
	{
		color = GetPixel(_mapPixel->getMemDC(), _x, i);	// for문을 돌면서 _y를 i로 검사
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);

		if (!_isFly || _state == FLYDOWN)	// 아래에서 위로 올라갈때 검사하면 안되니까 isFly가 false이거나 아래로 내려올때만 검사 한다.
		{
			if (!(r == 255 && g == 0 && b == 255))	// 마젠타가 아니면 검사
			{
				_y = i - (OTUS_HEIGTH / 2);	// _y는 i로 검사한 지점부터 오터스의 세로/2만큼 올려준다.
				_gravity = 0.f;				// 중력을 0으로 바꿔주고		
				_axisY = NONE;	// 계속 점프하는것을 방지
				_FY = FLY_N;	//
				_jumpCount = 0;	// 땅에 있으니까 점프카운트 0으로 바꿔준다.
				_isFly = false;	// 날지 않는 상태라는걸 바꿔주기
				if(_state != ROLL)
					_speed = 7;
				if (_state != WALK && _state != ATK && _state != ROLL && _state != LIFT )	// 뛰는상태가 아니고 공격상태가 아니면 IDLE
				{
					_state = IDLE;	// 언제 IDLE이여야 하는지 , 아니면 언제 IDLE로 바뀌면 안되는지 생각해보기
				}
				break;
			}
		}
		
	}	
	//왼쪽 검사
	for (int i = _hitBox.left + _speed; i >= _hitBox.left; i--)
	{
		color = GetPixel(_mapPixel->getMemDC(), i, _y);	// for문을 돌면서 _x를 i로 검사 
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);
	
		if ((r == 0 && g == 0 && b == 0)) // 검은색이면 검사, 검은색만 검사
		{
			_x = i + (OTUS_WIDTH / 2);
			break;
		}
	}
	//오른쪽 검사
	//for (int i = _hitBox.right; i < _hitBox.right + 1; i++)
	for (int i = _hitBox.right - _speed; i <= _hitBox.right; i++)
	{
		color = GetPixel(_mapPixel->getMemDC(), i, _y);
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);
		if ((r == 0 && g == 0 && b == 0))	// 마젠타가 아니면 검사 마젠타를 무시
		{
			_x = i - (OTUS_WIDTH / 2);
			break;
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
