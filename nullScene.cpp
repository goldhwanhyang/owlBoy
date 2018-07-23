#include "stdafx.h"
#include "nullScene.h"

HRESULT nullScene::init()
{
	/*
	//게임씬 초기화
	시작화면 클래스 생성및 초기화
	_startScene = new aaa;
	_startScene->init();
	//게임화면 클래스 생성및 초기화
	_gameScene = new bbb;
	_gameScene->init();
	//현재씬 초기화
	//_currentScene = _startScene;
	*/
	//백그라운드 이미지 초기화
	//_bgImage = new image;
	//_bgImage->init("Texture/background.bmp", WINSIZEX, WINSIZEY);
	IGM->addImage("배경", "Texture/background.bmp", WINSIZEX, WINSIZEY);
	IGM->addFrameImage("플레이어", "Texture/aladdin.bmp",float(WINSIZEX/2),float(WINSIZEY/2), 763, 124, 13, 2); //x,y는 실수형으로 명시할것
	//프레임 이미지 초기화

	//에너미매니져 클래스 생성 및 초기확
	/*
	_enemyManager = new enemyManager;
	_enemyManager->init();
	*/

	//사각형 위치 초기화

	CAM->setPosition(WINSIZEX/2, WINSIZEY/2); //카메라 초기좌표

	return S_OK;
}

void nullScene::release()
{
	//이미지 클래스 나갈때까진 사용할 일 없다
	//init함수에서 동적할당 new를 사용했다면 이곳에서 delete 사용한다

	/*
	//시작화면 게임화면 해제
	_startScene->release();
	SAFE_DELETE(_startScene);
	_gameScene->release();
	SAFE_DELETE(_gameScene);
	*/

	//에너미매니져 해제
	/*
	_enemyManager->release();
	SAFE_DELETE(_enemyManager);
	*/
}

void nullScene::update()
{
	//이곳에서 계산식, 키보드, 마우스등등 업데이트를 한다
	//간단하게 이곳에서 코딩한다고 생각하면 된다

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		CAM->setShakeInfo(20, 30);
	}

	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		EFFECT->generateEffect("플레이어", _ptMouse.x, _ptMouse.y, 0);
	}

	/*
	//현재씬만 업데이트
	_currentScene->update();

	if (KEYMANAGER->isOnceKeyDown('1'))
	{
	_currentScene = _startScene;
	_currentScene->init();
	}
	if (KEYMANAGER->isOnceKeyDown('2'))
	{
	_currentScene = _gameScene;
	_currentScene->init();
	}*/

	//적매니져 클래스 업데이트
	/*
	_enemyManager->update();
	*/
	CAM->videoShooting();	// 이전에 설정한 x, y 좌표를 촬영
	//CAM->videoShooting(player.x, player.y, player.speed);	// 플레이어의 x, y좌표를 설정, speed가 0이면 플레이어 위치를 곧장 쫓아감. speed가 0이 아니면 speed 속도로 쫓아감
}

void nullScene::render()
{
	//이곳에서 WM_PAINT에서 했던것을 처리하면 된다

	//백그라운드 이미지 렌더
	//방법1 -> 이미지클래스활용
	//_bgImage->render(getMemDC());
	//방법2 -> 이미지매니져,키값
	IGM->render("bgLoadingScene", getMemDC(), CAM->getSX(), CAM->getSY());
	IGM->frameRender("플레이어", getMemDC(), IGM->findI("플레이어")->getX() + CAM->getSX(), IGM->findI("플레이어")->getY() + CAM->getSY());

	//방법3 -> 이미지매니져,find (가능은함)
	//IMAGEMANAGER->findImage("배경)->render(getMemDC());

	// 캐릭터 프레임 이미지 렌더
	//_player->frameRender(memDC, _aladdin.bmp->getX(), _aladdin.bmp->getY());

	/*
	//현재씬만 렌더
	_currentScene->render();
	*/

	//에너미매니져 클래스 렌더
	/*
	_enemyManager->render();
	*/
	EFFECT->render(getMemDC());
}

void nullScene::collision()
{
	/*
	//충돌처리
	//플레이어의 총알 & 적 => 충돌처리
	//플레이어에서 총알클래스 가져온다
	//총알클래스에서 벡터 가져온다
	//플레이어->미사일클래스->총알벡터[]
	for (int i = 0; i < _player->getMissile()->getVBullet().size(); i++)
	{
	//RECT rc;//임시용렉트
	//RECT rcEnemy;
	//RECT rcBullet = _player->getMissile()->getVBullet()[i].rc;
	//if (IntersectRect(&rc, &rcBullet, &rcEnemy))
	//{
	//
	//}
	}

	//에너미클래스에 총알클래스가 있을때
	에너미매니져->에너미클래스->총알클래스->총알벡터

	//에너미매니져에서 총알클래스 들고 있을때
	에너미매니져->총알클래스->총알벡터

	*/
}
