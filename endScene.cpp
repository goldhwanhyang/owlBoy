#include "stdafx.h"
#include "endScene.h"


HRESULT endScene::init()
{
	_y = WINSIZEY;
	_count = _index[0] = _index[1] = _currentNum[0] = _currentNum[1] = 0;

	//_backGround = IMAGEMANAGER->findImage("endingBack");

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
	_rightFrame[1] = IMAGEMANAGER->findImage("�ź���_������1_�ȱ��");
	_rightFrame[2] = IMAGEMANAGER->findImage("�ź���_������1_���");
	_rightFrame[3] = IMAGEMANAGER->findImage("�ź���_������2_����");
	_rightFrame[4] = IMAGEMANAGER->findImage("�ź���_������2_���");
	_rightFrame[5] = IMAGEMANAGER->findImage("��ũ_����");
	_rightFrame[6] = IMAGEMANAGER->findImage("��ũ_���");
	_rightFrame[7] = IMAGEMANAGER->findImage("����1");
	_rightFrame[8] = IMAGEMANAGER->findImage("����2");
	_rightFrame[9] = IMAGEMANAGER->findImage("������");

	return S_OK;
}

void endScene::release()
{
}

void endScene::update()
{
	//_y -= 1;
	if (_y < -700)
	{
	}
	else
	{
		_y -= 1;
		//TODO �����̹���
	}
}

void endScene::render()
{
	_count = (_count + 1) % 15;
	creditsAni(_leftFrame, WINSIZEX / 6, 0);
	creditsAni(_rightFrame, WINSIZEX - 147, 1);
	creditsText();

}

void endScene::creditsText()
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

	//���ڻ���
	SetTextColor(getMemDC(), RGB(255, 255, 255));

	HFONT font;
	HFONT oldFont;
	font = CreateFont(25, 0, 0, 0, 1000, false, false, false, HANGEUL_CHARSET, 0, 0, 0, 0, TEXT("����"));
	oldFont = (HFONT)SelectObject(getMemDC(), font);
	char str[128];

	sprintf_s(str, "�� ��Ʈ������ [����] �ƿﺸ�̸� �÷��� ���ּż� �����մϴ�.");
	SetTextAlign(getMemDC(), TA_CENTER); //�ؽ�Ʈ �߾�����
	TextOut(getMemDC(), WINSIZEX / 2, _y, str, strlen(str));

	sprintf_s(str, "[TEAM] �� �츮�� ������");
	SetTextAlign(getMemDC(), TA_RIGHT); //�ؽ�Ʈ ����������
	TextOut(getMemDC(), WINSIZEX-140, _y+35, str, strlen(str));

	sprintf_s(str, "������");
	SetTextAlign(getMemDC(), TA_CENTER); //�ؽ�Ʈ �߾�����
	TextOut(getMemDC(), WINSIZEX / 2, _y + 135, str, strlen(str));

	sprintf_s(str, "���翵 ����");
	TextOut(getMemDC(), WINSIZEX / 2, _y + 235, str, strlen(str));

	sprintf_s(str, "����");
	TextOut(getMemDC(), WINSIZEX / 2, _y + 335, str, strlen(str));

	sprintf_s(str, "�ڵ���");
	TextOut(getMemDC(), WINSIZEX / 2, _y + 435, str, strlen(str));

	sprintf_s(str, "�ڼ��� ������ �����մϴ�.");
	TextOut(getMemDC(), WINSIZEX / 2, _y + 635, str, strlen(str));

	SelectObject(getMemDC(), oldFont);
	DeleteObject(font);
}

void endScene::creditsAni(image** ani, int x, int index)
{
	if (_count == 0)
	{
		++_index[index];
	}

	if (_index[index] > _leftFrame[_currentNum[index]]->getMaxFrameX())
	{
		_index[index] = 0;
		++_currentNum[index];
		if (_currentNum[index] > 9) _currentNum[index] = 0;
	}

	ani[_currentNum[index]]->frameRender(getMemDC(),
		x - ani[_currentNum[index]]->getFrameWidth()/2, 
		WINSIZEY  - ani[_currentNum[index]]->getFrameHeight()/2 - 297,
		_index[index], index);
}
