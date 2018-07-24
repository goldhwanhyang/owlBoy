#include "stdafx.h"
#include "mainGame.h"

//=============================================================
//	## �ʱ�ȭ ## init()
//=============================================================
HRESULT mainGame::init()
{
	gameNode::init(TRUE);
	//�̰����� �ʱ�ȭ�� �Ѵ�
	SCENEMANAGER->addScene("�ε�ȭ��", new loadingScene);
	SCENEMANAGER->addScene("��", new nullScene);
	SCENEMANAGER->addScene("playingScene", new playingScene);
	SCENEMANAGER->addScene("dungeonScene", new dungeonScene);

	SCENEMANAGER->loadScene("�ε�ȭ��");

	return S_OK;
}

//=============================================================
//	## ���� ## release()
//=============================================================
void mainGame::release()
{
	gameNode::release();
}

//=============================================================
//	## ������Ʈ ## update()
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
//	## ���� ## render()
//=============================================================
void mainGame::render() //���� �̹����Ŵ����� �߰��߰� gameNode�� �����ؼ� HDC�Ű����� ������ �ʿ䰡 ����.
{
	//��� �� ��Ʈ��(�̰͵� ������ �׳� �Ѱ�!!)
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
//=============================================================
	if (KEYM->isToggleKey(VK_F1))
	{
		_isDebug = TRUE; //��� Ŭ���� ����׸�� ����
	}

	SCENEMANAGER->render();

	//���Ű�� ���������� ���� ����Ѵ�
	if (_isDebug)
	{
		TextOut(getMemDC(), WINSIZEX / 2 - 40, 200, "����� ���", strlen("����� ���"));

	}
	_isDebug = FALSE; //��� Ŭ���� ����׸�� ����

//=============================================================	
	//������� ������ HDC�� �׸��� (�̰͵� ������ �׳� �Ѱ�!!)
	this->getBackBuffer()->render(getHDC());
}