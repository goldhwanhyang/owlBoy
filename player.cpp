#include "stdafx.h"
#include "player.h"

HRESULT player::init()
{
	//_testMap = IMAGEMANAGER->addImage("map", "Texture/player/1000x1000_map.bmp", 1000, 1000, true, RGB(255, 0, 255));
	_TownMap = IMAGEMANAGER->addImage("Town", "Texture/Maps/Town/townMap_5000x8000.bmp", 5000, 8000, true, RGB(255, 0, 255));
	_TownMapPixel = IMAGEMANAGER->addImage("TownPixel", "Texture/Maps/Town/townMapPixelCollision_5000x8000.bmp", 5000, 8000, true, RGB(255, 0, 255));
	CAM->setRange(_TownMap->getWidth(), _TownMap->getHeight());

	img[IDLE] = IMAGEMANAGER->addFrameImage("IDLE", "Texture/player/otusIdle_700x250_10x2.bmp", 700, 250, 10, 2);
	img[WALK] = IMAGEMANAGER->addFrameImage("Walk", "Texture/player/otusWalk_1000x270_8x2.bmp", 1000, 270, 8, 2);
	img[JUMP] = IMAGEMANAGER->addFrameImage("Jump", "Texture/player/otusJump_360x260_3x2.bmp", 360, 260, 3, 2);
	img[JUMPFALL] = IMAGEMANAGER->addFrameImage("Fall", "Texture/player/otusJumpFall_700x280_5x2.bmp", 700, 280, 5, 2);
	img[FLY] = IMAGEMANAGER->addFrameImage("FLY", "Texture/player/otusFly_1530x390_6x2.bmp", 1530, 390, 6, 2);

	_isLeft = false;	// 왼쪽 오른쪽
	_isWalk = false;	
	_isJump = false;
	_isFly = false;	
	_x = 540.f;			// 플레이어 x좌표
	_y = 465.f;			// 플레이어 y좌표
	_speed = 6.0f;		// 플레이어 속도
	_angle = PI / 2;
	_gravity = 0;		// 플레이어 중력
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH); 
	_count = _index = 0;	// 프레임이미지 돌리기 위한 초기화
	_weight = 0.f;
	_state = IDLE;

	return S_OK;
}

void player::release()
{

}

void player::update()
{
	if (_isJump == false)
	{
		_state = IDLE;
	}
	if (KEYMANAGER->isStayKeyDown('A'))
	{
		_x -= _speed;
		_isLeft = true;
		_state = WALK;
	}
	if (KEYMANAGER->isStayKeyDown('D'))
	{
		_x += _speed;
		_isLeft = false;
		_state = WALK;
	}
	if (KEYMANAGER->isStayKeyDown('W'))
	{
		_gravity -= 0.1f;
		_isJump = true;
		_state = JUMP;	
	}

	if (KEYMANAGER->isStayKeyDown('S'))
	{
		if(_isJump == true)
		_y += _speed;
	}

	if (_isJump == true)
	{
		_y += -sinf(_angle) * _speed + _gravity ;
		_state = JUMP;
	}
	if (_gravity > 6 )
	{
		_y += _gravity;
		_state = JUMPFALL;
	}

	_gravity += 0.16f;
	_hitBox = RectMakeCenter(_x, _y, OTUS_WIDTH, OTUS_HEIGTH);

	//위에 검사
	//for (int i = _hitBox.top; i < _hitBox.top + 1; i++)
	{
		COLORREF color = GetPixel(_TownMapPixel->getMemDC(), _x, _hitBox.top);
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
		COLORREF color = GetPixel(_TownMapPixel->getMemDC(), _x, _hitBox.bottom);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// 마젠타가 아니면 검사
		{
			_y = _hitBox.bottom - (OTUS_HEIGTH / 2);


			_gravity = 0.f;
			_weight = 0.f;
			_isJump = false;
			//break;
			//IDLE상태 일려면 바닥에 닿아있어야 하고 점프상태가 아니고 달리기 상태일 때
			_state = IDLE;
		}
	}
	//왼쪽 검사
	//for (int i = _hitBox.left; i < _hitBox.left + 1; i++)
	{
		COLORREF color = GetPixel(_TownMapPixel->getMemDC(), _hitBox.left, _y);
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
		COLORREF color = GetPixel(_TownMapPixel->getMemDC(), _hitBox.right, _y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))	// 마젠타가 아니면 검사
		{
			_x = _hitBox.right - (OTUS_WIDTH / 2 + _speed);
			//break;
		}
	}

	CAM->videoShooting(_x, _y);

	//아울보이 프레임 돌리기
	_count++;
	if (_isLeft == false)
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
	if (_isLeft == true)
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
}

void player::render()
{
	_TownMap->render(getMemDC(), CAM->getSX(), CAM->getSY(), CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY);
	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		_TownMapPixel->render(getMemDC(), CAM->getSX(), CAM->getSY(), CAM->getX(), CAM->getY(), WINSIZEX, WINSIZEY);
		Rectangle(getMemDC(), _hitBox.left - CAM->getX(), _hitBox.top - CAM->getY(), _hitBox.right - CAM->getX(), _hitBox.bottom - CAM->getY());
	}
	//cout << _gravity << endl;
	
	img[_state]->frameRender(getMemDC(), _x - CAM->getX() - img[_state]->getFrameWidth() / 2, _y - CAM->getY() - img[_state]->getFrameHeight() / 2);
	char str[128];
	sprintf_s(str, "X좌표 : %.3f Y좌표 : %.3f 중력 : %.3f", _x, _y,_gravity);
	TextOut(getMemDC(), 10, 10, str, strlen(str));
	//sprintf_s(str, "%d %d", _count, _index);
}

