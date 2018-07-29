#pragma once
#include "enemy.h"
#include "bullet.h"
#include "tortoiseShield.h"

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

	bool _isAttack;						//공격중
	int _attackCount;					//몇발쐈는지 체크
	int _delayCount;					//불릿 딜레이

	image* _tortoiseImage[MAX_STATE];	//보스 이미지[상태]			
	tortoiseShield* _shield;			//방패
	float _offSpeed;					//방패없을때 보스의 속도

	vector<bullet> _vBullet;			//불릿

	char _debug[128];
public:
	virtual void attack();
	virtual void move();
	virtual void collide();
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

	void setShieldLink(tortoiseShield* shield) { _shield = shield; }

	tortoise() {}
	~tortoise() {}
};

