#include "stdafx.h"
#include "loadingScene.h"

HRESULT loadingScene::init(void)
{
	//�ε�Ŭ���� �ʱ�ȭ
	_loading = new loading;
	_loading->init(); //�ε�ȭ�� �̹����� �ε� ���α׷�����

	//�̹��� �� ���� �ε�
	this->loadingImage();
	this->loadingSound();

	return S_OK;
}

void loadingScene::release(void)
{
	//�ε�Ŭ���� ����
	_loading->release();
	SAFE_DELETE(_loading);
}

void loadingScene::update(void)
{
	//�ε�Ŭ���� ������Ʈ
	_loading->update(); //�ε��� ������Ʈ

	//�ε��Ϸ��� ������
	if (_loading->loadingDone()) //loadingImage�� loadingSound�� ���ڰ����� addimage, addframeimage�ϰ� �� �Ǹ� true
	{
		SCENEMANAGER->loadScene("townScene"); //���Ӿ����� ��ȯ
	}
}

void loadingScene::render(void)
{
	//�ε�Ŭ���� ����
	_loading->render();
}

//�ε��̹��� �Լ�(�̰��� �̹����� ���� �־��)
void loadingScene::loadingImage()
{
	//_loading->loadImage();
	//_loading->loadFrameImage();
	//_loading->loadSound();

	////�ε��� �ʹ� ������ ����Ǽ� õõ�� ���ư����� �׽�Ʈ������ �����
	//for (int i = 0; i < 50; i++)
	//{
	//	_loading->loadImage(("�׽�Ʈ"+to_string(i)).c_str(), WINSIZEX, WINSIZEY);
	//}
	//IMAGEMANAGER->addImage("mountain", "mountain.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	//��׶��� �̹���
	//_loading->loadImage("mountain", "mountain.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	
	_loading->loadFrameImage("IDLE", "Texture/player/otusIdle_700x250_10x2.bmp", 700, 250, 10, 2);
	_loading->loadFrameImage("Walk", "Texture/player/otusWalk_1000x270_8x2.bmp", 1000, 270, 8, 2);
	_loading->loadFrameImage("Jump", "Texture/player/otusJump_360x260_3x2.bmp", 360, 260, 3, 2);
	_loading->loadFrameImage("Fall", "Texture/player/otusJumpFall_700x280_5x2.bmp", 700, 280, 5, 2);
	_loading->loadFrameImage("FLY", "Texture/player/otusFly_1530x390_6x2.bmp", 1530, 390, 6, 2);
	_loading->loadFrameImage("FLYDOWN", "Texture/player/otusFlyDown_585x360_3x2.bmp", 585, 360, 3, 2);
	_loading->loadFrameImage("ROLL", "Texture/player/otusRoll_1200x400_4x2.bmp", 1200, 400, 4, 2);

	_loading->loadFrameImage("����1����", "Texture/Enemies/Boss1/bossTurn_492x504_2x2.bmp", 492, 504, 2, 2);
	_loading->loadFrameImage("����1�ȱ�", "Texture/Enemies/Boss1/bossWalk_1968x504_8x2.bmp", 1968, 504, 8, 2);
	_loading->loadFrameImage("����1�ȱ��", "Texture/Enemies/Boss1/bossWalkShining_1968x504_8x2.bmp", 1968, 504, 8, 2);
	_loading->loadFrameImage("����1���", "Texture/Enemies/Boss1/bossShoot_1968x504_8x2.bmp", 1968, 504, 8, 2);

	_loading->loadFrameImage("ģ���������Ʈ", "Texture/Effect/takeFriendsEffect_1260x90_6x1.bmp", 1260, 90, 6, 1);
	_loading->loadFrameImage("�Ѿ���������Ʈ", "Texture/Effect/bulletExplodeEffect_1116x128_9x1.bmp", 1116, 128, 9, 1);
	_loading->loadFrameImage("ģ����ȯ����Ʈ", "Texture/Effect/callFriendsEffect_1680x300_6x1.bmp", 1680, 300, 6, 1);
	_loading->loadFrameImage("�������������Ʈ", "Texture/Effect/rollDustEffect_912x320_8x2.bmp", 912, 320, 8, 2);


	_loading->loadImage("������1", "Texture/Maps/Boss1/Map.bmp", 1920, 1080);
	_loading->loadImage("������1�ȼ�", "Texture/Maps/Boss1/MapPixel.bmp", 1920, 1080);

	_loading->loadImage("Town", "Texture/Maps/Town/townMap_5000x8000.bmp", 5000, 8000, true, RGB(255, 0, 255));
	_loading->loadImage("TownPixel", "Texture/Maps/Town/townMapPixelCollision_5000x8000.bmp", 5000, 8000, true, RGB(255, 0, 255));


}

//�ε����� �Լ�(�̰��� ���带 ���� �־��)
void loadingScene::loadingSound()
{
	_loading->loadSound("����1", "���.mp3");
	_loading->loadSound("����2", "���1.mp3");
}
