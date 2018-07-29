#pragma once
#include "enemy.h"
#include "bullet.h"
#define MAX_STATE 9

class tortoise : public enemy
{
private:
	enum STATE
	{
		READY,

		TURN,
		WALK,
		WALK_SHINING,
		ATTACK,

		TAKE_SHIELD,

		OFF_SHIELD,
		OFF_TURN,
		OFF_WALK
	};
	enum DIRECTION
	{
		RIGHT,
		LEFT
	};
	struct Shield
	{
		bool on;						//방패 챙겼니?
		float x;						//방패 x
		float y;						//방패 y
		RECT hitBox;					//방패 충돌박스
		int count;						//방패 프레임 카운트
		int index;						//방패 프레임 인덱스
		float offSpeed;					//방패가 없을때 속도
	};

	bool _isAttack;						//공격중
	int _attackCount;					//몇발쐈는지 체크
	int _delayCount;					//불릿 딜레이

	image* _tortoiseImage[MAX_STATE];	//보스 이미지[상태]
	image* _shieldDropImage;			//떨어진 방패 이미지
	Shield _shield;						//방패의 데이터

	vector<bullet> _vBullet;			//불릿

	char _debug[128];
public:
	virtual void attack();
	virtual void move();
	void turn();

	void shieldOff();
	void moveOff();
	//void turnOff();

	void takeShield();
	virtual void damaged(actor *e);

	void Bfire(float angle);
	void Bmove();
	void Brender();


	virtual HRESULT init(float x, float y);
	virtual void update();
	virtual void render();
	virtual void release();

	bool frameMake(image * bmp, int & count, int & index, int frameY1, int frameY2, int cooltime, bool renderDir);

	tortoise() {}
	~tortoise() {}
};

