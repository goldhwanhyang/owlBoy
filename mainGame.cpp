#include "stdafx.h"
#include "mainGame.h"

image * cursor;

//=============================================================
//	## 초기화 ## init()
//=============================================================
HRESULT mainGame::init()
{
	gameNode::init(TRUE);

	//글자배경모드
	SetBkMode(getMemDC(), TRANSPARENT);

	_soundVolume = INIDATA->loadDataFloat("Data/setting", "Sound", "master");
	_effectVolume = INIDATA->loadDataFloat("Data/setting", "Sound", "effect");

	//이곳에서 초기화를 한다
	SCENEMANAGER->addScene("로딩화면", new loadingScene);
	SCENEMANAGER->addScene("널", new nullScene);
	SCENEMANAGER->addScene("townScene", new townScene);
	SCENEMANAGER->addScene("dungeonScene", new dungeonScene);
	SCENEMANAGER->addScene("startScene", new startScene);
	SCENEMANAGER->addScene("endScene", new endScene);

	SCENEMANAGER->loadScene("로딩화면");

	cursor = IMAGEMANAGER->addImage("CURSOR", "Texture/UI/cursor_30x42.bmp", 30, 42, true, RGB(255, 0, 255));

	_player = new player;
	SAVEDATA->setPlayer(_player);

	return S_OK;
}

//=============================================================
//	## 해제 ## release()
//=============================================================
void mainGame::release()
{
	gameNode::release();

	_player->release();
	SAFE_DELETE(_player);


	INIDATA->addData("Sound", "master", to_string(_soundVolume).c_str());
	INIDATA->addData("Sound", "effect", to_string(_effectVolume).c_str());
	INIDATA->saveINI("Data/setting");
}

//=============================================================
//	## 업데이트 ## update()
//=============================================================
void mainGame::update()
{
	UIMANAGER->clear();

	SCENEMANAGER->update();
	EFFECTMANAGER->update();

	if (KEYMANAGER->isOnceKeyDown(VK_F7))
	{
		SCENEMANAGER->loadScene("townScene");
	}
	if (KEYMANAGER->isOnceKeyDown(VK_F6))
	{
		SCENEMANAGER->loadScene("dungeonScene");
	}

}

//=============================================================
//	## 렌더 ## render()
//=============================================================
void mainGame::render() //이제 이미지매니저를 추가했고 gameNode를 수정해서 HDC매개변수 가져올 필요가 없다.
{
	//흰색 빈 비트맵(이것도 렌더에 그냥 둘것!!)
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
//=============================================================
	if (KEYM->isToggleKey(VK_F1))
	{
		_isDebug = TRUE; //모든 클래스 디버그모드 시작
	}

	SCENEMANAGER->render();
	EFFECTMANAGER->render();
	//EFFECT->render(getMemDC());

	//토글키는 디버깅용으로 많이 사용한다
	if (_isDebug)
	{
		TextOut(getMemDC(), WINSIZEX / 2 - 40, 200, "디버그 모드", strlen("디버그 모드"));

	}
	_isDebug = FALSE; //모든 클래스 디버그모드 끄기
	TIMEMANAGER->render(getMemDC());

	UIMANAGER->render(getMemDC());

	cursor->render(getMemDC(), _ptMouse.x, _ptMouse.y);

//=============================================================	
	//백버퍼의 내용을 HDC에 그린다 (이것도 렌더에 그냥 둘것!!)
	this->getBackBuffer()->render(getHDC());
}