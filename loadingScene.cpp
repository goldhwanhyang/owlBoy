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
		loadingEffect();
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
	
	// background
	_loading->loadImage("����1", "Texture/background/cloud_462x174.bmp", 462, 174, true, RGB(255, 0, 255));
	_loading->loadImage("����2", "Texture/background/cloud2_789x478.bmp", 789, 478, true, RGB(255, 0, 255));
	_loading->loadImage("����3", "Texture/background/cloud3_484x201.bmp", 484, 201, true, RGB(255, 0, 255));
	_loading->loadImage("����4", "Texture/background/cloud4_537x185.bmp", 537, 185, true, RGB(255, 0, 255));
	_loading->loadImage("����5", "Texture/background/cloud5_319x286.bmp", 319, 286, true, RGB(255, 0, 255));
	_loading->loadImage("����6", "Texture/background/cloud6_402x235.bmp", 402, 235, true, RGB(255, 0, 255));
	_loading->loadImage("����7", "Texture/background/cloud7_510x255.bmp", 510, 255, true, RGB(255, 0, 255));
	_loading->loadImage("��1", "Texture/background/ston1_167x192.bmp", 167, 192, true, RGB(255, 0, 255));
	_loading->loadImage("��2", "Texture/background/ston2_189x171.bmp", 189, 171, true, RGB(255, 0, 255));

	// effect
	_loading->loadFrameImage("ģ���������Ʈ", "Texture/Effect/takeFriendsEffect_1260x90_6x1.bmp", 1260, 90, 6, 1);
	_loading->loadFrameImage("�Ѿ���������Ʈ", "Texture/Effect/bulletExplodeEffect_1116x128_9x1.bmp", 1116, 128, 9, 1);
	_loading->loadFrameImage("ģ����ȯ����Ʈ", "Texture/Effect/callFriendsEffect_1680x300_6x1.bmp", 1680, 300, 6, 1);
	_loading->loadFrameImage("�������������Ʈ", "Texture/Effect/rollDustEffect_912x320_8x2.bmp", 912, 320, 8, 2);
	_loading->loadFrameImage("�����������������Ʈ", "Texture/Effect/rollDustLeftEffect_912x160_8x1.bmp", 912, 160, 8, 1);
	_loading->loadFrameImage("�������������������Ʈ", "Texture/Effect/rollDustRightEffect_912x160_8x1.bmp", 912, 160, 8, 1);


	// player
	_loading->loadFrameImage("IDLE", "Texture/player/otusIdle_700x250_10x2.bmp", 700, 250, 10, 2);
	_loading->loadFrameImage("Walk", "Texture/player/otusWalk_1000x270_8x2.bmp", 1000, 270, 8, 2);
	_loading->loadFrameImage("Jump", "Texture/player/otusJump_360x260_3x2.bmp", 360, 260, 3, 2);
	_loading->loadFrameImage("Fall", "Texture/player/otusJumpFall_700x280_5x2.bmp", 700, 280, 5, 2);
	_loading->loadFrameImage("FLY", "Texture/player/otusFly_1530x390_6x2.bmp", 1530, 390, 6, 2);
	_loading->loadFrameImage("FLYDOWN", "Texture/player/otusFlyDown_585x360_3x2.bmp", 585, 360, 3, 2);
	_loading->loadFrameImage("ROLL", "Texture/player/otusRoll_1200x400_4x2.bmp", 1200, 400, 4, 2);

	_loading->loadFrameImage("ATTACK", "Texture/player/otusAttack_1350x260_5x2.bmp", 1350, 260, 5, 2);
	_loading->loadFrameImage("LIFT", "Texture/player/otusLift_1890x460_7x2.bmp", 1890, 460, 7, 2);
	_loading->loadFrameImage("LIFT2", "Texture/player/otusLiftOver_1890x460_7x2.bmp", 1890, 460, 7, 2);
	_loading->loadFrameImage("DAMAGED", "Texture/player/otusHit_1320x440_6x2.bmp", 1320, 440, 6, 2);
	_loading->loadFrameImage("THROWED", "Texture/player/otusThrowed_560x280_4x2.bmp", 560, 280, 4, 2);
	_loading->loadFrameImage("DEAD", "Texture/player/otusDead_5670x260_21x2.bmp", 5670, 260, 21, 2);

	// friends::geddy
	_loading->loadFrameImage("GEDDY_IDLE", "Texture/friends/geddyIdle_80x300_1x2.bmp", 80, 300, 1, 2);
	_loading->loadFrameImage("GEDDY_ON_AIR", "Texture/friends/geddyOnair_80x300_1x2.bmp", 80, 300, 1, 2);
	_loading->loadFrameImage("GEDDY_SHOOT", "Texture/friends/geddyShoot_960x1224_5x6.bmp", 960, 1224, 5, 6);
	_loading->loadFrameImage("GEDDY_WALK", "Texture/friends/geddyWalk_504x308_6x2.bmp", 504, 308, 6, 2);
	_loading->loadFrameImage("GEDDY_HAND", "Texture/friends/geddyShootingHand_1095x396_5x4.bmp", 1095, 396, 5, 4);
	_loading->loadFrameImage("GEDDY_PREPARE_SHOOT", "Texture/friends/geddyPrepareShooting_2112x408_11x2.bmp", 2112, 408, 11, 2);


	// enemy
	// tortoise
	_loading->loadFrameImage("�ź���_����������", "Texture/Enemies/Boss1/bossShieldDrop_3672x366_17_2.bmp", 3672, 366, 17, 2);
	_loading->loadFrameImage("�ź���_�Ҹ�", "Texture/Enemies/Boss1/bossBullet_525x138_7x2.bmp", 525, 138, 7, 2);
								   
	_loading->loadFrameImage("�ź���_������1_����", "Texture/Enemies/Boss1/bossTurn_492x504_2x2.bmp", 492, 504, 2, 2);
	_loading->loadFrameImage("�ź���_������1_�ȱ�", "Texture/Enemies/Boss1/bossWalk_1968x504_8x2.bmp", 1968, 504, 8, 2);
	_loading->loadFrameImage("�ź���_������1_�ȱ��", "Texture/Enemies/Boss1/bossWalkShining_1968x504_8x2.bmp", 1968, 504, 8, 2);
	_loading->loadFrameImage("�ź���_������1_���", "Texture/Enemies/Boss1/bossShoot_1968x504_8x2.bmp", 1968, 504, 8, 2);
								   		   
	_loading->loadFrameImage("�ź���_������1_�ȱ���о���", "Texture/Enemies/Boss1/bossOffWalk_1968x504_8x2.bmp", 1968, 504, 8, 2);
	_loading->loadFrameImage("�ź���_������1_���ж���߸���", "Texture/Enemies/Boss1/bossOffShield_522x504_2x2.bmp", 522, 504, 2, 2);
	_loading->loadFrameImage("�ź���_������1_����", "Texture/Enemies/Boss1/bossOffStun_522x504_2x2.bmp", 522, 504, 2, 2);
	_loading->loadFrameImage("�ź���_������1_������о���", "Texture/Enemies/Boss1/bossOffTurn_492x504_2x2.bmp", 492, 504, 2, 2);
	_loading->loadFrameImage("�ź���_������1_�����ݱ�", "Texture/Enemies/Boss1/bossTakeShield_1968x648_8x2.bmp", 1968, 648, 8, 2);
								   
	_loading->loadFrameImage("�ź���_������2_�غ�", "Texture/Enemies/Boss1/phase2Ready_2496x684_8x2", 2496, 684, 8, 2);
	_loading->loadFrameImage("�ź���_������2_����", "Texture/Enemies/Boss1/phase2Fly_2352x594_8x2.bmp", 2352, 594, 8, 2);
	_loading->loadFrameImage("�ź���_������2_���", "Texture/Enemies/Boss1/phase2Shoot_2352x594_8x2.bmp", 2352, 594, 8, 2);
	_loading->loadFrameImage("�ź���_������2_�����ݱ�", "Texture/Enemies/Boss1/phase2TakeShield_2448x684_8x2.bmp", 2448, 684, 8, 2);
	_loading->loadFrameImage("�ź���_������2_���ж���߸���", "Texture/Enemies/Boss1/phase2OffShield_636x420_2x2.bmp", 636, 420, 2, 2);
	_loading->loadFrameImage("�ź���_������2_����", "Texture/Enemies/Boss1/phase2OffStun_636x420_2x2.bmp", 636, 420, 2, 2);
	_loading->loadFrameImage("�ź���_������2_������о���", "Texture/Enemies/Boss1/phase2OffFly_2352x594_8x2.bmp", 2352, 594, 8, 2);


	// liftableActor
	_loading->loadFrameImage("������", "Texture/Stuff/weight_510x170_3x1.bmp", 510, 170, 3, 1);
	_loading->loadFrameImage("�����߰���", "Texture/Stuff/weight_510x170_3x1.bmp", 510, 170, 3, 1);
	_loading->loadImage("����1", "Texture/Stuff/fruit_60x95.bmp", 60, 95);
	_loading->loadImage("����2", "Texture/Stuff/fruit2_70x110.bmp", 70, 110);
	_loading->loadImage("��ư", "Texture/Stuff/button_114x44.bmp", 114, 44);
	_loading->loadImage("��ư��ħ", "Texture/Stuff/buttonBottom_145x30.bmp", 145, 30);


	// map
	_loading->loadImage("sky", "Texture/background/sky_1920x1080.bmp", WINSIZEX, WINSIZEY);
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

void loadingScene::loadingEffect()
{
	EFFECTMANAGER->addEffect("���", "ģ���������Ʈ", 0.5, 5);
	EFFECTMANAGER->addEffect("�Ѿ�����", "�Ѿ���������Ʈ", 0.5, 20);
	EFFECTMANAGER->addEffect("��ȯ", "ģ����ȯ����Ʈ", 0.5, 5);
	EFFECTMANAGER->addEffect("���������", "�������������Ʈ", 0.5, 10);
	EFFECTMANAGER->addEffect("���������", "�����������������Ʈ", 0.2, 10);
	EFFECTMANAGER->addEffect("�����������", "�������������������Ʈ", 0.2, 10);
}
