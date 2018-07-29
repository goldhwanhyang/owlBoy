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
	SCENEMANAGER->addScene("townScene", new townScene);
	SCENEMANAGER->addScene("dungeonScene", new dungeonScene);

	SCENEMANAGER->loadScene("�ε�ȭ��");


	_player = new player;
	SAVEDATA->setPlayer(_player);

	return S_OK;
}

//=============================================================
//	## ���� ## release()
//=============================================================
void mainGame::release()
{
	gameNode::release();

	_player->release();
	SAFE_DELETE(_player);
}

//=============================================================
//	## ������Ʈ ## update()
//=============================================================
void mainGame::update()
{

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
	RENDERMANAGER->render(getMemDC());
	EFFECTMANAGER->render();
	//EFFECT->render(getMemDC());

	//���Ű�� ���������� ���� ����Ѵ�
	if (_isDebug)
	{
		TextOut(getMemDC(), WINSIZEX / 2 - 40, 200, "����� ���", strlen("����� ���"));

	}
	_isDebug = FALSE; //��� Ŭ���� ����׸�� ����
	TIMEMANAGER->render(getMemDC());

//=============================================================	
	//������� ������ HDC�� �׸��� (�̰͵� ������ �׳� �Ѱ�!!)
	this->getBackBuffer()->render(getHDC());
}