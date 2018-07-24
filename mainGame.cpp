#include "stdafx.h"
#include "mainGame.h"

//=============================================================
//	## 초기화 ## init()
//=============================================================
HRESULT mainGame::init()
{
	gameNode::init(TRUE);
	//이곳에서 초기화를 한다
	SCENEMANAGER->addScene("로딩화면", new loadingScene);
	SCENEMANAGER->addScene("널", new nullScene);
	SCENEMANAGER->addScene("playingScene", new playingScene);
	SCENEMANAGER->addScene("dungeonScene", new dungeonScene);

	SCENEMANAGER->loadScene("로딩화면");

	return S_OK;
}

//=============================================================
//	## 해제 ## release()
//=============================================================
void mainGame::release()
{
	gameNode::release();
}

//=============================================================
//	## 업데이트 ## update()
//=============================================================
void mainGame::update()
{
	gameNode::update();

	if (KEYMANAGER->isOnceKeyDown(VK_F5))
	{
		SCENEMANAGER->loadScene("playingScene");
	}
	if (KEYMANAGER->isOnceKeyDown(VK_F6))
	{
		SCENEMANAGER->loadScene("dungeonScene");
	}

	SCENEMANAGER->update();
}

//=============================================================
//	## 렌더 ## render()
//=============================================================
void mainGame::render() //이제 이미지매니저를 추가했고 gameNode를 수정해서 HDC매개변수 가져올 필요가 없다.
{
	//흰색 빈 비트맵(이것도 렌더에 그냥 둘것!!)
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
//=============================================================
	if (KEYM->isToggleKey(VK_F1))
	{
		_isDebug = TRUE; //모든 클래스 디버그모드 시작
	}

	SCENEMANAGER->render();

	//토글키는 디버깅용으로 많이 사용한다
	if (_isDebug)
	{
		TextOut(getMemDC(), WINSIZEX / 2 - 40, 200, "디버그 모드", strlen("디버그 모드"));

	}
	_isDebug = FALSE; //모든 클래스 디버그모드 끄기

//=============================================================	
	//백버퍼의 내용을 HDC에 그린다 (이것도 렌더에 그냥 둘것!!)
	this->getBackBuffer()->render(getHDC());
}