#include "stdafx.h"
#include "endScene.h"


HRESULT endScene::init()
{
	_y = WINSIZEY;
	_count = 0;
	return S_OK;
}

void endScene::release()
{
}

void endScene::update()
{
	//_count = (_count + 1) % 10;
	if (_count == 0)
	{
		_y -= 1;
	}

	if (_y < -500)
	{
		SCENEMANAGER->loadScene("startScene");
	}
}

void endScene::render()
{

	/*
	font = CreateFont(
	문자 크기, 문자 폭, 문자 기울기, 문자 방향, 문자 굵기,
	기울기(bool), 밑줄(bool), 취소선(bool),
	문자 세팅, 출력 정확도, 클리핑 정확도, 출력의 질, 자간, 폰트
	)
	*/
	// 문자 폭은 문자크기에 따라 비례하게 표시. 0으로 두는 것이 좋음

	// 폰트 사용법
	HFONT font;
	HFONT oldFont;
	font = CreateFont(20, 0, 0, 0, 600, false, false, false, HANGEUL_CHARSET, 0, 0, 0, 0, TEXT("휴먼굴림"));
	oldFont = (HFONT)SelectObject(getMemDC(), font);
	char str[128];
	
	
	sprintf_s(str, "OwlBoy 팀 포트폴리오를 플레이 해주셔서 감사합니다.");
	TextOut(getMemDC(), 100, _y, str, strlen(str));

	sprintf_s(str, "제작자");
	TextOut(getMemDC(), WINSIZEX / 2, _y + 100, str, strlen(str));

	sprintf_s(str, "이재영 팀장");
	TextOut(getMemDC(), WINSIZEX / 2, _y + 200, str, strlen(str));

	sprintf_s(str, "김상균");
	TextOut(getMemDC(), WINSIZEX / 2, _y + 300, str, strlen(str));

	sprintf_s(str, "박동건");
	TextOut(getMemDC(), WINSIZEX / 2, _y + 400, str, strlen(str));


	SelectObject(getMemDC(), oldFont);
	DeleteObject(font);
	
}
