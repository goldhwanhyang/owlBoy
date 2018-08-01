#include "stdafx.h"
#include "loadingScene.h"

HRESULT loadingScene::init(void)
{
	//로딩클래스 초기화
	_loading = new loading;
	_loading->init(); //로딩화면 이미지와 로딩 프로그래스바

	//이미지 및 사운드 로딩
	this->loadingImage();
	this->loadingSound();

	return S_OK;
}

void loadingScene::release(void)
{
	//로딩클래스 해제
	_loading->release();
	SAFE_DELETE(_loading);
}

void loadingScene::update(void)
{
	//로딩클래스 업데이트
	_loading->update(); //로딩바 업데이트

	//로딩완료후 씬변경
	if (_loading->loadingDone()) //loadingImage와 loadingSound의 인자값으로 addimage, addframeimage하고 다 되면 true
	{
		loadingEffect();
		SCENEMANAGER->loadScene("townScene"); //게임씬으로 전환
	}
}

void loadingScene::render(void)
{
	//로딩클래스 렌더
	_loading->render();
}

//로딩이미지 함수(이곳에 이미지를 전부 넣어라)
void loadingScene::loadingImage()
{
	//_loading->loadImage();
	//_loading->loadFrameImage();
	//_loading->loadSound();

	////로딩이 너무 빠르게 진행되서 천천히 돌아가도록 테스트용으로 만들기
	//for (int i = 0; i < 50; i++)
	//{
	//	_loading->loadImage(("테스트"+to_string(i)).c_str(), WINSIZEX, WINSIZEY);
	//}
	//IMAGEMANAGER->addImage("mountain", "mountain.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	//백그라운드 이미지
	//_loading->loadImage("mountain", "mountain.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
	
	// background
	_loading->loadImage("구름1", "Texture/background/cloud_462x174.bmp", 462, 174, true, RGB(255, 0, 255));
	_loading->loadImage("구름2", "Texture/background/cloud2_789x478.bmp", 789, 478, true, RGB(255, 0, 255));
	_loading->loadImage("구름3", "Texture/background/cloud3_484x201.bmp", 484, 201, true, RGB(255, 0, 255));
	_loading->loadImage("구름4", "Texture/background/cloud4_537x185.bmp", 537, 185, true, RGB(255, 0, 255));
	_loading->loadImage("구름5", "Texture/background/cloud5_319x286.bmp", 319, 286, true, RGB(255, 0, 255));
	_loading->loadImage("구름6", "Texture/background/cloud6_402x235.bmp", 402, 235, true, RGB(255, 0, 255));
	_loading->loadImage("구름7", "Texture/background/cloud7_510x255.bmp", 510, 255, true, RGB(255, 0, 255));
	_loading->loadImage("돌1", "Texture/background/ston1_167x192.bmp", 167, 192, true, RGB(255, 0, 255));
	_loading->loadImage("돌2", "Texture/background/ston2_189x171.bmp", 189, 171, true, RGB(255, 0, 255));

	// effect
	_loading->loadFrameImage("친구들기이펙트", "Texture/Effect/takeFriendsEffect_1260x90_6x1.bmp", 1260, 90, 6, 1);
	_loading->loadFrameImage("총알폭발이펙트", "Texture/Effect/bulletExplodeEffect_1116x128_9x1.bmp", 1116, 128, 9, 1);
	_loading->loadFrameImage("친구소환이펙트", "Texture/Effect/callFriendsEffect_1680x300_6x1.bmp", 1680, 300, 6, 1);
	_loading->loadFrameImage("구르기먼지이펙트", "Texture/Effect/rollDustEffect_912x320_8x2.bmp", 912, 320, 8, 2);
	_loading->loadFrameImage("구르기먼지왼쪽이펙트", "Texture/Effect/rollDustLeftEffect_912x160_8x1.bmp", 912, 160, 8, 1);
	_loading->loadFrameImage("구르기먼지오른쪽이펙트", "Texture/Effect/rollDustRightEffect_912x160_8x1.bmp", 912, 160, 8, 1);


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
	_loading->loadFrameImage("거북이_떨어진방패", "Texture/Enemies/Boss1/bossShieldDrop_3672x366_17_2.bmp", 3672, 366, 17, 2);
	_loading->loadFrameImage("거북이_불릿", "Texture/Enemies/Boss1/bossBullet_525x138_7x2.bmp", 525, 138, 7, 2);
								   
	_loading->loadFrameImage("거북이_페이즈1_꺽기", "Texture/Enemies/Boss1/bossTurn_492x504_2x2.bmp", 492, 504, 2, 2);
	_loading->loadFrameImage("거북이_페이즈1_걷기", "Texture/Enemies/Boss1/bossWalk_1968x504_8x2.bmp", 1968, 504, 8, 2);
	_loading->loadFrameImage("거북이_페이즈1_걷기빛", "Texture/Enemies/Boss1/bossWalkShining_1968x504_8x2.bmp", 1968, 504, 8, 2);
	_loading->loadFrameImage("거북이_페이즈1_쏘기", "Texture/Enemies/Boss1/bossShoot_1968x504_8x2.bmp", 1968, 504, 8, 2);
								   		   
	_loading->loadFrameImage("거북이_페이즈1_걷기방패없이", "Texture/Enemies/Boss1/bossOffWalk_1968x504_8x2.bmp", 1968, 504, 8, 2);
	_loading->loadFrameImage("거북이_페이즈1_방패떨어뜨리기", "Texture/Enemies/Boss1/bossOffShield_522x504_2x2.bmp", 522, 504, 2, 2);
	_loading->loadFrameImage("거북이_페이즈1_스턴", "Texture/Enemies/Boss1/bossOffStun_522x504_2x2.bmp", 522, 504, 2, 2);
	_loading->loadFrameImage("거북이_페이즈1_꺽기방패없이", "Texture/Enemies/Boss1/bossOffTurn_492x504_2x2.bmp", 492, 504, 2, 2);
	_loading->loadFrameImage("거북이_페이즈1_방패줍기", "Texture/Enemies/Boss1/bossTakeShield_1968x648_8x2.bmp", 1968, 648, 8, 2);
								   
	_loading->loadFrameImage("거북이_페이즈2_준비", "Texture/Enemies/Boss1/phase2Ready_2496x684_8x2", 2496, 684, 8, 2);
	_loading->loadFrameImage("거북이_페이즈2_날기", "Texture/Enemies/Boss1/phase2Fly_2352x594_8x2.bmp", 2352, 594, 8, 2);
	_loading->loadFrameImage("거북이_페이즈2_쏘기", "Texture/Enemies/Boss1/phase2Shoot_2352x594_8x2.bmp", 2352, 594, 8, 2);
	_loading->loadFrameImage("거북이_페이즈2_방패줍기", "Texture/Enemies/Boss1/phase2TakeShield_2448x684_8x2.bmp", 2448, 684, 8, 2);
	_loading->loadFrameImage("거북이_페이즈2_방패떨어뜨리기", "Texture/Enemies/Boss1/phase2OffShield_636x420_2x2.bmp", 636, 420, 2, 2);
	_loading->loadFrameImage("거북이_페이즈2_스턴", "Texture/Enemies/Boss1/phase2OffStun_636x420_2x2.bmp", 636, 420, 2, 2);
	_loading->loadFrameImage("거북이_페이즈2_날기방패없이", "Texture/Enemies/Boss1/phase2OffFly_2352x594_8x2.bmp", 2352, 594, 8, 2);


	// liftableActor
	_loading->loadFrameImage("무게추", "Texture/Stuff/weight_510x170_3x1.bmp", 510, 170, 3, 1);
	_loading->loadFrameImage("무게추공중", "Texture/Stuff/weight_510x170_3x1.bmp", 510, 170, 3, 1);
	_loading->loadImage("과일1", "Texture/Stuff/fruit_60x95.bmp", 60, 95);
	_loading->loadImage("과일2", "Texture/Stuff/fruit2_70x110.bmp", 70, 110);
	_loading->loadImage("버튼", "Texture/Stuff/button_114x44.bmp", 114, 44);
	_loading->loadImage("버튼받침", "Texture/Stuff/buttonBottom_145x30.bmp", 145, 30);


	// map
	_loading->loadImage("sky", "Texture/background/sky_1920x1080.bmp", WINSIZEX, WINSIZEY);
	_loading->loadImage("보스방1", "Texture/Maps/Boss1/Map.bmp", 1920, 1080);
	_loading->loadImage("보스방1픽셀", "Texture/Maps/Boss1/MapPixel.bmp", 1920, 1080);

	_loading->loadImage("Town", "Texture/Maps/Town/townMap_5000x8000.bmp", 5000, 8000, true, RGB(255, 0, 255));
	_loading->loadImage("TownPixel", "Texture/Maps/Town/townMapPixelCollision_5000x8000.bmp", 5000, 8000, true, RGB(255, 0, 255));


}

//로딩사운드 함수(이곳에 사운드를 전부 넣어라)
void loadingScene::loadingSound()
{
	_loading->loadSound("사운드1", "브금.mp3");
	_loading->loadSound("사운드2", "브금1.mp3");
}

void loadingScene::loadingEffect()
{
	EFFECTMANAGER->addEffect("들기", "친구들기이펙트", 0.5, 5);
	EFFECTMANAGER->addEffect("총알폭발", "총알폭발이펙트", 0.5, 20);
	EFFECTMANAGER->addEffect("소환", "친구소환이펙트", 0.5, 5);
	EFFECTMANAGER->addEffect("구르기먼지", "구르기먼지이펙트", 0.5, 10);
	EFFECTMANAGER->addEffect("구르기왼쪽", "구르기먼지왼쪽이펙트", 0.2, 10);
	EFFECTMANAGER->addEffect("구르기오른쪽", "구르기먼지오른쪽이펙트", 0.2, 10);
}
