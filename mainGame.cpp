#include "stdafx.h"
#include "mainGame.h"

image * cursor;

//=============================================================
//	## �ʱ�ȭ ## init()
//=============================================================
HRESULT mainGame::init()
{
	gameNode::init(TRUE);

	//���ڹ����
	SetBkMode(getMemDC(), TRANSPARENT);

	_soundVolume = INIDATA->loadDataFloat("Data/setting", "Sound", "master");
	_effectVolume = INIDATA->loadDataFloat("Data/setting", "Sound", "effect");

	//�̰����� �ʱ�ȭ�� �Ѵ�
	SCENEMANAGER->addScene("�ε�ȭ��", new loadingScene);
	SCENEMANAGER->addScene("��", new nullScene);
	SCENEMANAGER->addScene("townScene", new townScene);
	SCENEMANAGER->addScene("dungeonScene", new dungeonScene);
	SCENEMANAGER->addScene("startScene", new startScene);
	SCENEMANAGER->addScene("endScene", new endScene);

	SCENEMANAGER->loadScene("�ε�ȭ��");

	cursor = IMAGEMANAGER->addImage("CURSOR", "Texture/UI/cursor_30x42.bmp", 30, 42, true, RGB(255, 0, 255));

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


	INIDATA->addData("Sound", "master", to_string(_soundVolume).c_str());
	INIDATA->addData("Sound", "effect", to_string(_effectVolume).c_str());
	INIDATA->saveINI("Data/setting");
}

//=============================================================
//	## ������Ʈ ## update()
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
//	## ���� ## render()
//=============================================================
void mainGame::render() //���� �̹����Ŵ����� �߰��߰� gameNode�� �����ؼ� HDC�Ű����� ������ �ʿ䰡 ����.
{
	//��� �� ��Ʈ��(�̰͵� ������ �׳� �Ѱ�!!)
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
//=============================================================
	if (KEYM->isToggleKey(VK_F1))
	{
		_isDebug = TRUE; //��� Ŭ���� ����׸�� ����
	}

	SCENEMANAGER->render();
	EFFECTMANAGER->render();
	//EFFECT->render(getMemDC());

	//���Ű�� ���������� ���� ����Ѵ�
	if (_isDebug)
	{
		TextOut(getMemDC(), WINSIZEX / 2 - 40, 200, "����� ���", strlen("����� ���"));

	}
	_isDebug = FALSE; //��� Ŭ���� ����׸�� ����
	TIMEMANAGER->render(getMemDC());

	UIMANAGER->render(getMemDC());

	cursor->render(getMemDC(), _ptMouse.x, _ptMouse.y);

//=============================================================	
	//������� ������ HDC�� �׸��� (�̰͵� ������ �׳� �Ѱ�!!)
	this->getBackBuffer()->render(getHDC());
}