#pragma once
#include "actor.h"

#define OTUS_WIDTH 50
#define OTUS_HEIGTH 125
#define PI 3.14
#define STATE_MAX 10	// 상태 추가 될때마다 까먹지말고 갯수 늘려주기
enum STATE { IDLE , WALK, JUMP, JUMPFALL , FLY , FLYDOWN, ROLL , ATK, LIFT, LIFT2 };
enum WAY { RIGHT , LEFT, UP, DOWN, NONE};	// 왼쪽, 오른쪽 , 위 , 아래 , 아무것도 누르지 않았을 때
enum FLYING { FLY_L, FLY_R, FLY_U, FLY_D, FLY_N };

//불값으로는 
class player : public actor
{
private:
	int _beforeState;
	WAY _axisX, _axisY;	// 어떤키가 눌렸는지 , 아무것도 안눌렸는지 확인
	FLYING _FX, _FY;	//
	image* img[STATE_MAX];
	image* _mapPixel;
	image* _map;

	bool _isLeft;
	bool _isFly;	// 날고있는 상태인지 아닌지
	int _jumpCount; // 땅에있는지 날고있는지 구분하기위한 변수


	float _jumpSpeed;	// 점프 스피드 중력
	float _flySpeed;	// 날고있을 때 속도
	float _rollSpeed;	// 구르기 속도

	int _oldY;	// 이전 위치
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



	void collide();
	void frameSetting();


	void setMap(image *map) { _map = map; }
	void setMapPixel(image *mapPixel) { _mapPixel = mapPixel; }
	player(){}
	~player(){}
};