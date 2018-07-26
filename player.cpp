#include "stdafx.h"
#include "player.h"

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

	_isLeft = false;	// true = 왼쪽 , false = 오른쪽
	_isWalk = false;	// true = 걷는 모션
	_isJump = false;	// true = 점프하는 모션
	_isFly = false;		// true = 날라다니는 모션
	_isFlyDown = false;	// true = 공중에서 아래로 내려가는 모션
	_jumpCount = 0;

	_x = 440.f;			// 플레이어 x좌표
	_y = 810.f;			// 플레이어 y좌표
	_speed = 6.0f;		// 플레이어 속도
	_angle = PI / 2;	// 플레이어 각도
	_gravity = 0;		// 플레이어 중력
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH); 
	_count = _index = 0;	// 프레임이미지 돌리기 위한 초기화

	_state = IDLE;		// 기본 상태 = 가만히있는 상태
	_beforeState = _state;

	return S_OK;
}

void player::release()
{

}

void player::update()
{
	if (_isJump == false && _isWalk == false && _state != ROLL) //걷기,점프가 false이고 ROLL상태가 아닐 때 IDLE상태프레임으로 바꾼다.
	{
		_state = IDLE;
	}
	this->inputKey();
	_gravity += 0.16f;		// 중력값은 0.16씩 더해준다.
	if (_isJump == true)	// _isJump가 true면 점프가 되고 점프하는 프레임을 보여준다.
	{
		_y += -sinf(_angle) * _speed + _gravity ;	
		_state = JUMP;
	}
	if (_gravity > 1.5f )	// 중력값이 6 이상이 되면 ( 원래는 0보다 커지면인데 모션이 빨리나와서 6으로 변경 ) 아래로 중력값을 더해주고 아래로 내려오는 모션을 보여준다.
	{
		_y += _gravity;		
		_state = JUMPFALL;	
		_jumpCount = 1;
	}
	if (_isFly == true )	// _isFly == true일 때 점프가 안되게 false로 바꿔주고 날고있는 모션을 보여준다. 떨어지지 않게 중력값을 0으로 준다.
	{
		if (_state != ROLL)
		{
			_state = FLY;
		}	
		_isJump = false;
		_gravity = 0;
		// 날고있는 상태에서는 무조건 _state가 FLY라서 ROLL이 안됨
		// _state가 FLY로 돼서 ROLL상태가 안됨~ 이거를 구르기 상태로 바꿔주려면?
		// _state 를 ROLL로 바꿔주기 스페이스바를 누르면 굴러야함!
		// 어떻게 조건을 줘야 할까 
	}

	if (_isFlyDown == true)	// _isFlyDown == true일 때 아래로 내려가는 모션을 보여준다.
	{
		if(_state != ROLL)
		_state = FLYDOWN;
	}
	if (_state == ROLL)	// 구를 때 움직임 처리
	{
		if (!_isLeft) // 오른쪽일 때
		{
			_x -= cosf(_angle * 2) * _speed * 2;
			//_x +=  _speed * 2 + 1;
			//if (_state == FLYDOWN)
			//{
			//	_y -= -sinf(_angle * 2) * _speed * 2;
			//}
		}
		if (_isLeft) // 왼쪽일 때
		{
			_x += cosf(_angle * 2) * _speed * 2;
			//_x -= _speed * 2 + 1;
		}
	}
	//fly상태 일때 스페이스바를 누르면 굴러야 한다.
	
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
	img[_state]->frameRender(getMemDC(), _x - CAM->getX() - img[_state]->getFrameWidth() / 2, _y - CAM->getY() - img[_state]->getFrameHeight() / 2);
	char str[128];
	sprintf_s(str, "X좌표 : %.3f Y좌표 : %.3f 중력 : %.3f 인덱스 : %d", _x, _y,_gravity, _index);
	TextOut(getMemDC(), 10, 10, str, strlen(str));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//키입력함수
void player::inputKey()
{
	if (_state != ROLL)
	{
		if (KEYMANAGER->isStayKeyDown('A'))	// A키를 누르면 왼쪽으로 스피드만큼 이동하고 왼쪽으로 뛰는프레임을 보여준다.
		{
			_x += cosf(_angle * 2) * _speed;
			//_x -= _speed;
			_isLeft = true;
			_state = WALK;
		}
		if (KEYMANAGER->isStayKeyDown('D')) // D키를 누르면 오른쪽으로 스피드만큼 이동하고 오른쪽으로 뛰는 프레임을 보여준다.
		{
			_x -= cosf(_angle * 2) * _speed ;
			//_x += _speed;
			_isLeft = false;
			_state = WALK;
		}
		if (KEYMANAGER->isStayKeyDown('W')) // W키를 오래 누르고 있을수록 높이 점프하게끔 중력값을 빼주고 점프 프레임을 돌리기 위해 _isjump를 true로 바꿔준다.
		{
			_gravity -= 0.1f;
			_isJump = true;
		}
		if (KEYMANAGER->isOnceKeyDown('W'))
		{
			_jumpCount += 1;
			if (_jumpCount > 2)	// 점프카운트가 2보다 커지면 2로 고정
			{
				_jumpCount = 2;
			}
			if (_jumpCount == 2)	// 점프카운트가 2가 되면 날라다니는 프레임이 나오게 _isFly를 true로 바꿔준다.
			{
				_isFly = true;
			}
		}
		if (KEYMANAGER->isStayKeyDown('S') && _isFly == true)
		{
			// IDLE일때도 내려가고 , 점프상태에서 FLYDOWN이 되고 ,

			_isFlyDown = true;		// 아래로 내려갈 때 true가 되고
			if (_isFly == true || _state == FLYDOWN )	// 날고 있거나 상태가 아래로 내려가는 상태일 때
			{
				_y += _speed;	// _y에 스피드만큼 더해준다. (아래로 내려감 )
			}
		}
		if (KEYMANAGER->isOnceKeyUp('S'))	// S키에서 손이 눌렸다 떼지면 _isFlyDown = false;
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


//충돌함수
void player::collide()
{
	//위에 검사
	//for (int i = _hitBox.top; i < _hitBox.top + 1; i++)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.top);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		//isFly일때 isFlyDown이 아닐 때 충돌 검사
		if ((r == 0 && g == 0 && b == 0)) // 검은색만 검사
		{
			_y = _hitBox.top + (OTUS_HEIGTH / 2 + _speed);
			//break;
		}
	}
	//아래 검사
	//for (int i = _hitBox.bottom-10; i < _hitBox.bottom; i++)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.bottom);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// 마젠타가 아니면 검사
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
			//공중에서 날고있는 상태일 때
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
	//왼쪽 검사
	//for (int i = _hitBox.left; i < _hitBox.left + 1; i++)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _hitBox.left, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if ((r == 0 && g == 0 && b == 0)) // 마젠타가 아니면 검사 였다가 검은색이면 검사
		{
			_x = _hitBox.left + (OTUS_WIDTH / 2 + _speed);
			//break;
		}
	}
	//오른쪽 검사
	//for (int i = _hitBox.right; i < _hitBox.right + 1; i++)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _hitBox.right, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if ((r == 0 && g == 0 && b == 0))	// 마젠타가 아니면 검사 마젠타를 무시
											//검은색은 안지나가고 초록색은 지나치게 할려면 
		{
			_x = _hitBox.right - (OTUS_WIDTH / 2 + _speed);
			//break;
		}
	}

}


//이미지프레임셋팅
void player::frameSetting()
{
	_count++;
	// 프레임 두가지종류?를 돌리는 이유는 아직 완전히 바꾸지 않아서 임시로 나눠서 돌리기
	if (_state == ROLL)		// 구르는 상태일 때
	{
		if(_isLeft)
			img[ROLL]->setFrameY(1);
		else
			img[ROLL]->setFrameY(0);
		if (_count % 5 == 0)	// 카운트를 5로 나눠서 0이 될때마다
		{
			_index++;			// 인덱스를 1씩 빼고
			if (_index > img[_state]->getMaxFrameX())		// 인덱스가 0보다 작아지려고 하면
			{
				_state = _beforeState;	// 이전상태로
				_index = 0;
			}
			img[_state]->setFrameX(_index);

		}
		return;
	}

	if (_isLeft == false)		// 오른쪽
	{
		if (_state != JUMPFALL && _state != ROLL)			// 떨어지는 상태가 아닐때 모두 이 if문을 돌리고
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
		if (_state == JUMPFALL && _state != ROLL)	// 떨어지는 상태일때만 이 이프문을 돌린다.
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
		//if (_state == ROLL)			// 상태가 구르는 상태 일 때
		//{
		//	img[ROLL]->setFrameY(0); 
		//	if (_count % 5 == 0)
		//	{
		//		_index++;
		//		if (_index > img[ROLL]->getMaxFrameX())	// 맥스프레임X보다 커지면
		//		{
		//			_state = _beforeState;	// 상태 = 이전상태
		//			//_isRoll = false;
		//			//_state = IDLE;
		//		}
		//		img[ROLL]->setFrameX(_index);
		//	}
		//}
	}
	if (_isLeft == true)			// 왼쪽
	{
		if (_state != JUMPFALL && _state != ROLL)		// 떨어지는 상태가 아닐 때, 구르는 상태가 아닐 때 나머지 프레임을 돌린다 ( 뛰기 , 점프 등등)
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
		if (_state == JUMPFALL && _state != ROLL)	// 떨어지는 상태일 때
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
		//if (_state == ROLL)		// 구르는 상태일 때
		//{
		//	img[ROLL]->setFrameY(1);
		//	if (_count % 5 == 0)	// 카운트를 5로 나눠서 0이 될때마다
		//	{
		//		_index--;			// 인덱스를 1씩 빼고
		//		if (_index < 0)		// 인덱스가 0보다 작아지려고 하면
		//		{
		//			_state = _beforeState;	// 이전상태로
		//		}
		//		img[ROLL]->setFrameX(_index);
		//	}
		//}
	}
}
