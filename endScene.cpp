#include "stdafx.h"
#include "endScene.h"


HRESULT endScene::init()
{
	_y = WINSIZEY;
	_count = _index[0] = _index[1] = _currentNum[0] = _currentNum[1] = 0;

	_leftFrame[0] = IMAGEMANAGER->findImage("IDLE");
	_leftFrame[1] = IMAGEMANAGER->findImage("Walk");
	_leftFrame[2] = IMAGEMANAGER->findImage("Jump");
	_leftFrame[3] = IMAGEMANAGER->findImage("Fall");
	_leftFrame[4] = IMAGEMANAGER->findImage("FLY");
	_leftFrame[5] = IMAGEMANAGER->findImage("FLYDOWN");
	_leftFrame[6] = IMAGEMANAGER->findImage("ROLL");
	_leftFrame[7] = IMAGEMANAGER->findImage("ATTACK");
	_leftFrame[8] = IMAGEMANAGER->findImage("THROWED");
	_leftFrame[9] = IMAGEMANAGER->findImage("NPC");

	_rightFrame[0] = IMAGEMANAGER->findImage("GEDDY_WALK");
	_rightFrame[1] = IMAGEMANAGER->findImage("거북이_페이즈1_걷기빛");
	_rightFrame[2] = IMAGEMANAGER->findImage("거북이_페이즈1_쏘기");
	_rightFrame[3] = IMAGEMANAGER->findImage("거북이_페이즈2_날기");
	_rightFrame[4] = IMAGEMANAGER->findImage("거북이_페이즈2_쏘기");
	_rightFrame[5] = IMAGEMANAGER->findImage("고크_날기");
	_rightFrame[6] = IMAGEMANAGER->findImage("토크_쏘기");
	_rightFrame[7] = IMAGEMANAGER->findImage("나무1");
	_rightFrame[8] = IMAGEMANAGER->findImage("나무2");
	_rightFrame[9] = IMAGEMANAGER->findImage("무게추");

	return S_OK;
}

void endScene::release()
{
}

void endScene::update()
{
	_y -= 1;
	if (_y < -700)
	{
		_y -= 1;
	}
	else
	{
		//TODO 엔딩이미지
	}
}

void endScene::render()
{
	creditsText();
	_count = (_count + 1) % 10;
	if (_count == 0)
	{
		++_index[0];
		++_index[1];
	}

	if (_index[0] > _leftFrame[_currentNum[0]]->getMaxFrameX())
	{
		_index[0] = 0;
		++_currentNum[0];
	}

	if (_index[1] > _rightFrame[_currentNum[1]]->getMaxFrameX())
	{
		_index[1] = 0;
		++_currentNum[1];
	}

	_leftFrame[_currentNum[0]]->frameRender(getMemDC(), _leftFrame[_currentNum[0]]->getFrameWidth()/2, WINSIZEY - _leftFrame[_currentNum[0]]->getFrameHeight()/2, _index[0], 0);
	_rightFrame[_currentNum[1]]->frameRender(getMemDC(), WINSIZEX - _rightFrame[_currentNum[1]]->getFrameWidth()/2, WINSIZEY - _rightFrame[_currentNum[1]]->getFrameHeight()/2, _index[1], 1);
}

void endScene::creditsText()
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

	//글자색상
	SetTextColor(getMemDC(), RGB(255, 255, 255));

	HFONT font;
	HFONT oldFont;
	font = CreateFont(25, 0, 0, 0, 1000, false, false, false, HANGEUL_CHARSET, 0, 0, 0, 0, TEXT("돋움"));
	oldFont = (HFONT)SelectObject(getMemDC(), font);
	char str[128];

	sprintf_s(str, "OwlBoy 팀 포트폴리오를 플레이 해주셔서 감사합니다.");
	SetTextAlign(getMemDC(), TA_CENTER); //텍스트 중앙정렬
	TextOut(getMemDC(), WINSIZEX / 2, _y, str, strlen(str));

	sprintf_s(str, "제작자");
	TextOut(getMemDC(), WINSIZEX / 2, _y + 100, str, strlen(str));

	sprintf_s(str, "이재영 팀장");
	TextOut(getMemDC(), WINSIZEX / 2, _y + 200, str, strlen(str));

	sprintf_s(str, "김상균");
	TextOut(getMemDC(), WINSIZEX / 2, _y + 300, str, strlen(str));

	sprintf_s(str, "박동건");
	TextOut(getMemDC(), WINSIZEX / 2, _y + 400, str, strlen(str));

	sprintf_s(str, "박성우 선생님 감사합니다.");
	TextOut(getMemDC(), WINSIZEX / 2, _y + 600, str, strlen(str));

	SelectObject(getMemDC(), oldFont);
	DeleteObject(font);
}
