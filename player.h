#pragma once
#include "actor.h"				// 상속의 부모클래스
#include "geddy.h"
#include "progressBar.h"		// 체력바 클래스 사용을 위해 가져온다.
// liftableActor
// stuffMAnager상호참조

class enemyManager;	// enemyManager라는 클래스가 있다는것을 알리기 위해 전방선언을 해주었다.
class stuffManager;	// stuffManager라는 클래스가 있다는것을 알리기 위해 전방선언을 해주었다.

#define OTUS_WIDTH 50
#define OTUS_HEIGTH 125
#define PI 3.14
#define STATE_MAX 11	// 상태 추가 될때마다 까먹지말고 갯수 늘려주기
enum STATE { IDLE , WALK, JUMP, JUMPFALL , FLY , FLYDOWN, ROLL , ATK, LIFT,HIT,DEAD };
enum WAY { RIGHT , LEFT, UP, DOWN, NONE};	// 왼쪽, 오른쪽 , 위 , 아래 , 아무것도 누르지 않았을 때
enum FLYING { FLY_L, FLY_R, FLY_U, FLY_D, FLY_N };

//불값으로는 
class player : public actor
{
private:
	enemyManager* _enemyManager;
	stuffManager* _stuffManager;

	/* 
		포인터 : 틀을 통해서 다른곳에 만들어져 있는 에너미매니져를 사용하겠다. ( 에너미매니져가 setPlayer(_player) 등의 정보를 가져와서 틀에 담는다 )
		번거롭게 mainGame이나 Scene을 거치지 않고 간단하게 서로 직접 접근 가능하게 해준다.	
	*/
	liftableActor* _liftableActor;
	geddy* _geddy;
	progressBar* _hpBar;	

	int _beforeState;
	WAY _axisX, _axisY;	// 어떤키가 눌렸는지 , 아무것도 안눌렸는지 확인
	FLYING _FX, _FY;	//
	image* img[STATE_MAX];
	int _frameCount[STATE_MAX];

	image* _liftImg;
	image* _mapPixel;
	image* _map;

	image* friendsFace;
	image* coin;

	RECT _spinHitBox;
	RECT _rollHitBox;
	bool _isKnockBack;
	bool _isBack;
	bool _isLeft;
	bool _isFly;	// 날고있는 상태인지 아닌지
	bool _isDead;
	int _jumpCount; // 땅에있는지 날고있는지 구분하기위한 변수
	int _coin;
	int _maxCoin;

	float _jumpSpeed;	// 점프 스피드 중력
	float _flySpeed;	// 날고있을 때 속도
	float _rollSpeed;	// 구르기 속도
	float _knockBackSpeed;
	float _backSpeed;

	int _oldX;	// 이전 x위치
	int _oldY;	// 이전 y위치
	int _walkY;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	//땅에 있을 때
	void groundInputKey();							// 땅에 있을 때 키 입력 ( 어떤 키를 눌러서 어떤키가 눌렸는지 확인해주기 )
	void groundAxis(WAY axisX, WAY axisY);			// 눌린 키가 몇개인지 어떤 키가 눌렸는지 확인하고 상태주기
	//void groundMove();								// Axis로 바뀐 상태에 따라 움직여주기

	//날고 있을 때
	void flyInputKey();
	float flyAngle(FLYING FX, FLYING FY);
	void flyMove();

	//상태가 바뀔때마다 index를 0으로 초기화해주는 함수
	void changeState(int state);

	void commonInputKey();
	void collide();
	void collideMap();
	void collideActor();
	void collideStuff();

	// 플레이어가 대미지를 입으면 자기 자신의 피를 깎는다
	virtual void damaged(actor *e);	// 체력 감소, 넉백, 동료 떨어트리기(오투스), 서있을 때 피격시 사라짐(게디, 알폰스)
	
	void frameSetting();

	void setMap(image *map) { _map = map; }
	void setMapPixel(image *mapPixel) { _mapPixel = mapPixel; }
	void setEnemyManager(enemyManager* e) { _enemyManager = e; }	// 틀만 있던 _enemyManager에 다른곳에있는 에너미매니져의정보를 가져오기위한 함수.
	void setStuffManager(stuffManager* s) { _stuffManager = s; }	// 틀만 있던 _stuffManager에 다른곳에있는 스터프매니져의정보를 가져오기위한 함수.
	void setliftableActor(liftableActor* l) { _liftableActor = l; }
	void setGeddy(geddy *g) { _geddy = g; }

	int getCoin() { return _coin; }
	void setCoin(int coin) { _coin = coin; }
	// 코인 함수 증가시키기 숫자넣으면 코인의값을 증가시키는 함수
	int sumCoin(int coin);

	bool isChangableToIdle() { return (_state != WALK && _state != ATK && _state != ROLL && _state != LIFT && _state != HIT && _state != DEAD); };

	// 예시 ) 컵에 물을 따르려면 가만히두면 물이 안따라지니까 손이라는 도구를 사용해서 물을 따른다
	// 컵 = 틀 , 물 = 알맹이, 손 = set함수
	player():_stuffManager(NULL), _liftableActor(NULL), _geddy(NULL), _hpBar(NULL), _coin(-1){}
	~player(){}
};