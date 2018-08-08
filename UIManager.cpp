#include "stdafx.h"
#include "UIManager.h"



HRESULT UIManager::init(void)
{
	_uiDC = new image;
	_uiDC->init(WINSIZEX, WINSIZEY);

	return S_OK;
}

void UIManager::release(void)
{
	_uiDC->release();
	SAFE_DELETE(_uiDC);
}

void UIManager::update()
{
}

void UIManager::render(HDC hdc)
{
	//GdiTransparentBlt : 비트맵의 특정색상을 제외하고 고속복사 해주는 함수
	GdiTransparentBlt(
		hdc,					//복사할 장소의 DC
		0,					//복사될 좌표 시작점 X
		0,					//복사될 좌표 시작점 Y
		WINSIZEX,		//복사될 이미지 가로크기
		WINSIZEY,		//복사될 이미지 세로크기
		_uiDC->getMemDC(),		//복사될 대상 DC
		0, 0,					//복사 시작지점
		WINSIZEX,		//복사 영역 가로크기
		WINSIZEY,		//복사 영역 세로크기
		RGB(255, 0, 255));			//복사할때 제외할 색상 (마젠타)
}

void UIManager::clear()
{
	HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255));
	SelectObject(_uiDC->getMemDC(), brush);
	Rectangle(_uiDC->getMemDC(), RectMake(0, 0, WINSIZEX, WINSIZEY));
	DeleteObject(brush);
	
}
