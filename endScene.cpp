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
	���� ũ��, ���� ��, ���� ����, ���� ����, ���� ����,
	����(bool), ����(bool), ��Ҽ�(bool),
	���� ����, ��� ��Ȯ��, Ŭ���� ��Ȯ��, ����� ��, �ڰ�, ��Ʈ
	)
	*/
	// ���� ���� ����ũ�⿡ ���� ����ϰ� ǥ��. 0���� �δ� ���� ����

	// ��Ʈ ����
	HFONT font;
	HFONT oldFont;
	font = CreateFont(20, 0, 0, 0, 600, false, false, false, HANGEUL_CHARSET, 0, 0, 0, 0, TEXT("�޸ձ���"));
	oldFont = (HFONT)SelectObject(getMemDC(), font);
	char str[128];
	
	
	sprintf_s(str, "OwlBoy �� ��Ʈ�������� �÷��� ���ּż� �����մϴ�.");
	TextOut(getMemDC(), 100, _y, str, strlen(str));

	sprintf_s(str, "������");
	TextOut(getMemDC(), WINSIZEX / 2, _y + 100, str, strlen(str));

	sprintf_s(str, "���翵 ����");
	TextOut(getMemDC(), WINSIZEX / 2, _y + 200, str, strlen(str));

	sprintf_s(str, "����");
	TextOut(getMemDC(), WINSIZEX / 2, _y + 300, str, strlen(str));

	sprintf_s(str, "�ڵ���");
	TextOut(getMemDC(), WINSIZEX / 2, _y + 400, str, strlen(str));


	SelectObject(getMemDC(), oldFont);
	DeleteObject(font);
	
}
