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

	_isLeft = false;	// true = 왼쪽 , false = 오른쪽
	_isWalk = false;	// true = 걷는프레임
	_isJump = false;	// true = 점프하는 프레임
	_isFly = false;		// true = 날라다니는 프레임
	_jumpCount = 0;

	_x = 440.f;			// 플레이어 x좌표
	_y = 810.f;			// 플레이어 y좌표
	_speed = 6.0f;		// 플레이어 속도
	_angle = PI / 2;	// 플레이어 각도
	_gravity = 0;		// 플레이어 중력
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH); 
	_count = _index = 0;	// 프레임이미지 돌리기 위한 초기화

	_state = IDLE;		// 기본 상태 = 가만히있는 상태

	return S_OK;
}

void player::release()
{

}

void player::update()
{
	if (_isJump == false && _isWalk == false) // 점프가 false, false일 때 IDLE상태프레임으로 바꾼다.
	{
		_state = IDLE;
	}
	if (KEYMANAGER->isStayKeyDown('A'))	// A키를 누르면 왼쪽으로 스피드만큼 이동하고 왼쪽으로 뛰는프레임을 보여준다.
	{
		_x -= _speed;
		_isLeft = true;
		_state = WALK;
	}
	if (KEYMANAGER->isStayKeyDown('D')) // D키를 누르면 오른쪽으로 스피드만큼 이동하고 오른쪽으로 뛰는 프레임을 보여준다.
	{
		_x += _speed;
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
	if (KEYMANAGER->isStayKeyDown('S')) 
	{
		_state = FLYDOWN;
		if (_isFly == true || _state == FLYDOWN)
		{
			_y += _speed;
		}
	}


	_gravity += 0.16f;		// 중력값은 0.16씩 더해준다.
	if (_isJump == true)	// _isJump가 true면 점프가 되고 점프하는 프레임을 보여준다.
	{
		_y += -sinf(_angle) * _speed + _gravity ;
		_state = JUMP;
	}
	if (_gravity > 6 )	// 중력값이 6 이상이 되면 ( 원래는 0보다 커지면인데 모션이 빨리나와서 6으로 변경 ) 아래로 중력값을 더해주고 아래로 내려오는 모션을 보여준다.
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
	// 아래 상하좌우 검사하기전에 히트박스를 먼저 갱신 해주고 검사를한다.
	// 오투스의 렉트 
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);	

	//위에 검사
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
	//아래 검사
	//for (int i = _hitBox.bottom; i < _hitBox.bottom+1; i++)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _x, _hitBox.bottom);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// 마젠타가 아니면 검사
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
	//왼쪽 검사
	//for (int i = _hitBox.left; i < _hitBox.left + 1; i++)
	{
		COLORREF color = GetPixel(_mapPixel->getMemDC(), _hitBox.left, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// 마젠타가 아니면 검사
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

		if (!(r == 255 && g == 0 && b == 255))	// 마젠타가 아니면 검사
		{
			_x = _hitBox.right - (OTUS_WIDTH / 2 + _speed);
			//break;
		}
	}

	//아울보이 프레임 돌리기
	_count++;
	if (_isLeft == false)		// 오른쪽
	{
		if (_state != JUMPFALL)			// 떨어지는 상태가 아닐때 모두 이 if문을 돌리고
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
		if (_state == JUMPFALL)	// 떨어지는 상태일때만 이 이프문을 돌린다
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
	if (_isLeft == true)			// 왼쪽
	{		
		if (_state != JUMPFALL)		// 떨어지는 상태가 아닐 때 나머지 프레임  ( 뛰기 , 점프 등등)
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
		if (_state == JUMPFALL)	// 떨어지는 상태일 때
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
	sprintf_s(str, "X좌표 : %.3f Y좌표 : %.3f 중력 : %.3f 점프카운트 : %d", _x, _y,_gravity, _jumpCount);
	TextOut(getMemDC(), 10, 10, str, strlen(str));
	//sprintf_s(str, "%d %d", _count, _index);
}

