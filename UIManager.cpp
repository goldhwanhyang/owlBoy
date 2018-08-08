#include "stdafx.h"
#include "UIManager.h"
#include "progressBar.h"



HRESULT UIManager::init(void)
{

	//image
	IMAGEMANAGER->addImage("VOLUME_BACK", "Texture/UI/volumeProgressBack_673x81.bmp", 673, 81, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("VOLUME_FRONT", "Texture/UI/volumeProgressFront_641x66.bmp", 641, 66, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("VOLUME_SETTING", "Texture/UI/volumeSetting_800x269.bmp", 800, 269, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("BACK_TO_MENU", "Texture/Text/backtoMenu_456x63.bmp", 456, 63, true, RGB(255, 0, 255));



	_uiDC = new image;
	_uiDC->init(WINSIZEX, WINSIZEY);

	_uiType = 0;
	_isDrawUI = false;
	_isBlockingUI = false;


	/////////// volume UI
	_backToMenu = IMAGEMANAGER->findImage("BACK_TO_MENU");
	_board = IMAGEMANAGER->findImage("VOLUME_SETTING");

	_board->setX(WINSIZEX / 2 - _board->getWidth() / 2);
	_board->setY(WINSIZEY / 2 - _board->getHeight() / 2);

	_backToMenu->setX(_board->getX());
	_backToMenu->setY(_board->getY() - _backToMenu->getHeight() - 50);

	_volume = new progressBar;
	_volumeX = WINSIZEX / 2 - 25;
	_volumeY = WINSIZEY / 2 - _board->getY() / 4;
	_volume->init("Texture/UI/volumeProgressFront_641x66", "Texture/UI/volumeProgressBack_673x81", _volumeX, _volumeY, _board->getWidth() / 2 - 25, 50);
	_volume->setGauge(_soundVolume * 100, 100);
	_volumeWidth = 0;


	return S_OK;
}

void UIManager::release(void)
{
	_uiDC->release();
	SAFE_DELETE(_uiDC);
}

void UIManager::update()
{
	if (KEYMANAGER->isOnceKeyDown(VK_ESCAPE))
	{
		_isDrawUI = true;
	}
	if (!_isDrawUI) return;

	switch (_uiType)
	{
	case 0:

		break;
	case 1:

		break;
	}
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

void UIManager::drawVolumeSetting()
{
	_board->render(_uiDC->getMemDC(), _board->getX(), _board->getY());
	_volume->render();
	_backToMenu->render(_uiDC->getMemDC(), _backToMenu->getX(), _backToMenu->getY());
}
