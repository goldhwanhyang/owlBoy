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
	//GdiTransparentBlt : ��Ʈ���� Ư�������� �����ϰ� ��Ӻ��� ���ִ� �Լ�
	GdiTransparentBlt(
		hdc,					//������ ����� DC
		0,					//����� ��ǥ ������ X
		0,					//����� ��ǥ ������ Y
		WINSIZEX,		//����� �̹��� ����ũ��
		WINSIZEY,		//����� �̹��� ����ũ��
		_uiDC->getMemDC(),		//����� ��� DC
		0, 0,					//���� ��������
		WINSIZEX,		//���� ���� ����ũ��
		WINSIZEY,		//���� ���� ����ũ��
		RGB(255, 0, 255));			//�����Ҷ� ������ ���� (����Ÿ)
}

void UIManager::clear()
{
	HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255));
	SelectObject(_uiDC->getMemDC(), brush);
	Rectangle(_uiDC->getMemDC(), RectMake(0, 0, WINSIZEX, WINSIZEY));
	DeleteObject(brush);
	
}
