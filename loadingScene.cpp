#include "stdafx.h"
#include "loadingScene.h"

HRESULT loadingScene::init(void)
{
	//로딩클래스 초기화
	_loading = new loading;
	_loading->init(); //로딩화면 이미지와 로딩 프로그래스바

	_line = IMAGEMANAGER->addImage("LOADING_BAR", "Texture/Loading/loadingBar_72x454.bmp", 72, 454, true, RGB(255, 0, 255));
	_lineBottom = IMAGEMANAGER->addImage("LOADING_BAR_BOTTOM", "Texture/Loading/loadingBarBottom_72x28.bmp", 72, 28, true, RGB(255, 0, 255));
	_otus = IMAGEMANAGER->addFrameImage("LIFT", "Texture/player/otusLift_1890x460_7x2.bmp", 1890, 460, 7, 2);
	_otusOver = IMAGEMANAGER->addFrameImage("LIFT2", "Texture/player/otusLiftOver_1890x460_7x2.bmp", 1890, 460, 7, 2);

	_count = _index = _height = 0;


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
	_count = (_count + 1) % 10;
	if (_count == 0)
	{
		_index = (_index + 1) % (_otus->getMaxFrameX() + 1);
	}
	//로딩클래스 업데이트
	_loading->update(); //로딩바 업데이트

	//로딩완료후 씬변경
	if (_loading->loadingDone() && (KEYMANAGER->isOnceKeyDown(VK_SPACE) || KEYMANAGER->isOnceKeyDown(VK_LBUTTON))) //loadingImage와 loadingSound의 인자값으로 addimage, addframeimage하고 다 되면 true
	{
		loadingEffect();
		SCENEMANAGER->loadScene("startScene"); //게임씬으로 전환
	}
}

void loadingScene::render(void)
{
	//로딩클래스 렌더
	_loading->render();
	/*
	if (_loading->getCurrnetGauge() < _loading->getLoadItem().size())
	{
		char str[128];
		sprintf_s(str, "%s.bmp", _loading->getLoadItem()[_loading->getCurrnetGauge()]->getImageResource().keyName.c_str());
		TextOut(getMemDC(), 120, 430, str, strlen(str));
	}

	float num = _loading->getLoadItem().size();
	float cur = _loading->getCurrnetGauge();
	float per = cur / num * 100;
	char str[128];
	sprintf_s(str, "%.f %%", per);
	TextOut(getMemDC(), 660, 430, str, strlen(str));
	*/
	float num = _loading->getLoadItem().size();
	float cur = _loading->getCurrnetGauge();
	float per = cur / num;
	_height = _line->getHeight() * per;

	_otus->frameRender(getMemDC(),
		WINSIZEX / 2 - _otus->getFrameWidth() / 2,
		WINSIZEY - _height - _otus->getFrameHeight() / 2, _index, 0);

	_line->render(getMemDC(),
		WINSIZEX / 2 - _lineBottom->getWidth() / 2,
		WINSIZEY - _height - 10, 0, 0, _line->getWidth(), _height);

	_otusOver->frameRender(getMemDC(),
		WINSIZEX / 2 - _otus->getFrameWidth() / 2,
		WINSIZEY - _height - _otus->getFrameHeight() / 2, _index, 0);


	_lineBottom->render(getMemDC(),
		WINSIZEX / 2 - _lineBottom->getWidth() / 2,
		WINSIZEY - _lineBottom->getHeight() - 10);

	if (per >= 1)
	{
		char str[128];
		sprintf_s(str, "press space");
		TextOut(getMemDC(), WINSIZEX/2 - 50, WINSIZEY/2, str, strlen(str));
	}
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
	
	//Text
	_loading->loadImage("BACK", "Texture/Text/back_135x39.bmp", 135, 39, true, RGB(255, 0, 255));
	_loading->loadImage("BACK_TO_GAME", "Texture/Text/backtoGame_285x39.bmp", 285, 39, true, RGB(255, 0, 255));
	_loading->loadImage("BACK_TO_MENU", "Texture/Text/backtoMenu_456x63.bmp", 456, 63, true, RGB(255, 0, 255));
	_loading->loadImage("COIN", "Texture/Text/coin_66x39.bmp", 66, 39, true, RGB(255, 0, 255));
	_loading->loadImage("CONTINUE", "Texture/Text/continue_135x39.bmp", 135, 39, true, RGB(255, 0, 255));
	_loading->loadImage("GAME_EXIT", "Texture/Text/gameExit_150x39.bmp", 150, 39, true, RGB(255, 0, 255));
	_loading->loadImage("GO_TO_TITLE", "Texture/Text/gotoStart_408x39.bmp", 408, 39, true, RGB(255, 0, 255));
	_loading->loadImage("ITEM", "Texture/Text/item_99x39.bmp", 99, 39, true, RGB(255, 0, 255));
	_loading->loadImage("OPTION", "Texture/Text/option_69x39.bmp", 69, 39, true, RGB(255, 0, 255));
	_loading->loadImage("SYSTEM", "Texture/Text/system_99x39.bmp", 99, 39, true, RGB(255, 0, 255));
	_loading->loadImage("TITLE", "Texture/Text/title_597x303.bmp", 597, 303, true, RGB(255, 0, 255));

	//UI
	_loading->loadImage("CURSOR", "Texture/UI/cursor_30x42.bmp", 30, 42, true, RGB(255, 0, 255));
	_loading->loadImage("ATTACK_CURSOR", "Texture/UI/attackCursor_81x81.bmp", 81, 81, true, RGB(255, 0, 255));
	_loading->loadImage("EXIT_MENU", "Texture/UI/exitMenu_1110x90.bmp", 1110, 90, true, RGB(255, 0, 255));
	_loading->loadImage("FRIEND_UI", "Texture/UI/friendGeddyUI_75x69.bmp", 75, 69, true, RGB(255, 0, 255));
	_loading->loadImage("HP_BACK", "Texture/UI/hpBarBack_223x36.bmp", 223, 36, true, RGB(255, 0, 255));
	_loading->loadImage("HP_FRONT", "Texture/UI/hpBarFront_220x27.bmp", 220, 27, true, RGB(255, 0, 255));
	_loading->loadImage("MOUSE_LEFT", "Texture/UI/leftClickUI_54x60.bmp", 54, 60, true, RGB(255, 0, 255));
	_loading->loadImage("MOUSE_RIGHT", "Texture/UI/rightClickUI_54x60.bmp", 54, 60, true, RGB(255, 0, 255));
	_loading->loadImage("VOLUME_BACK", "Texture/UI/volumeProgressBack_673x81.bmp", 673, 81, true, RGB(255, 0, 255));
	_loading->loadImage("VOLUME_FRONT", "Texture/UI/volumeProgressFront_641x66.bmp", 641, 66, true, RGB(255, 0, 255));
	_loading->loadImage("VOLUME_RIGHT", "Texture/UI/volumeProgressFrontRight_19x66.bmp", 19, 50, true, RGB(255, 0, 255));
	//_loading->loadImage("VOLUME_SETTING", "Texture/UI/volumeSetting_1473x495.bmp", 1473, 495, true, RGB(255, 0, 255));
	_loading->loadImage("VOLUME_SETTING", "Texture/UI/volumeSetting_800x269.bmp", 800, 269, true, RGB(255, 0, 255));


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

	// background - 시작메뉴
	_loading->loadImage("START_SCENE_BACKGROUND", "Texture/background/startBackground_50x50.bmp", WINSIZEX, WINSIZEY);
	_loading->loadImage("START_SCENE_BACKGROUND_CLOUD", "Texture/background/startBackgroundCloud_800x800.bmp", 600, 600);
	_loading->loadFrameImage("START_SCENE_OTUS", "Texture/background/startSceneBack2_1500x551_3x1.bmp", 1500, 551, 3, 1);
	_loading->loadImage("START_SCENE_STONE", "Texture/background/startSceneBack3_500x500.bmp", 500, 500, true, RGB(255, 0, 255));
	



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

	_loading->loadImage("SHADOW", "Texture/player/shadow_54x6.bmp", 54, 6, true, RGB(255, 0, 255));

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
								   
	_loading->loadFrameImage("거북이_페이즈2_준비", "Texture/Enemies/Boss1/phase2Ready_2496x684_8x2.bmp", 2496, 684, 8, 2);
	_loading->loadFrameImage("거북이_페이즈2_날기", "Texture/Enemies/Boss1/phase2Fly_2352x594_8x2.bmp", 2352, 594, 8, 2);
	_loading->loadFrameImage("거북이_페이즈2_쏘기", "Texture/Enemies/Boss1/phase2Shoot_2352x594_8x2.bmp", 2352, 594, 8, 2);
	_loading->loadFrameImage("거북이_페이즈2_방패줍기", "Texture/Enemies/Boss1/phase2TakeShield_2448x684_8x2.bmp", 2448, 684, 8, 2);
	_loading->loadFrameImage("거북이_페이즈2_방패떨어뜨리기", "Texture/Enemies/Boss1/phase2OffShield_636x420_2x2.bmp", 636, 420, 2, 2);
	_loading->loadFrameImage("거북이_페이즈2_스턴", "Texture/Enemies/Boss1/phase2OffStun_636x420_2x2.bmp", 636, 420, 2, 2);
	_loading->loadFrameImage("거북이_페이즈2_날기방패없이", "Texture/Enemies/Boss1/phase2OffFly_2352x594_8x2.bmp", 2352, 594, 8, 2);


	// liftableActor
	_loading->loadFrameImage("무게추", "Texture/Stuff/weight_510x170_3x1.bmp", 510, 170, 3, 1);
	_loading->loadImage("무게추공중", "Texture/Stuff/weightOnAir_140x170.bmp", 140, 170, true, RGB(255, 0, 255));
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


	// stuff- Ring
	_loading->loadFrameImage("FRONT_RING", "Texture/Stuff/stonRingFront_700x1440_2x4.bmp", 700, 1440, 2, 4);
	_loading->loadFrameImage("BACK_RING", "Texture/Stuff/stonRingBack_700x1440_2x4.bmp", 700, 1440, 2, 4);

}

//로딩사운드 함수(이곳에 사운드를 전부 넣어라)
void loadingScene::loadingSound()
{
	_loading->loadSound("시작", "SOUND/OST/Owlboy_Main_Title.mp3", true, true);
	_loading->loadSound("던전", "SOUND/OST/OwlTemple.mp3", true, true);
	_loading->loadSound("상점", "SOUND/OST/shop.mp3", true, true);
	_loading->loadSound("보스_페이즈1", "SOUND/OST/Turtle_Guardian_Phase_1.mp3", true, true);
	_loading->loadSound("보스_페이즈2", "SOUND/OST/Turtle_Guardian_Phase_2.mp3", true, true);
	_loading->loadSound("마을", "SOUND/OST/Vellie.mp3", true, true);
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
