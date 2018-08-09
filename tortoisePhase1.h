#pragma once
#include "enemy.h"
#include "progressBar.h"
#include "bullet.h"
#include "tortoiseShield.h"

namespace PHASE1_CONST
{
	const int MAX_STATE = 10;
	const float DEFAULT_OFF_SPEED = 2.8f;
	const float MAX_OFF_SPEED = 8.0f;
	const int HITBOX_WIDTH = 220;
	const int HITBOX_HEIGHT = 160;
}

class tortoisePhase1 : public enemy
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
		OFF_STUN,
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

	image* _tortoiseImage[PHASE1_CONST::MAX_STATE];	//보스 이미지[상태]
	image* _tortoiseDamagedImg[PHASE1_CONST::MAX_STATE]; //보스 피격 이미지
	int _damAlpha;
	int _damAlphaBlue;
	int _alphaDelay;
	int _alphaCount;
	bool _isAlpha;

	tortoiseShield* _shield;			//방패

	float _offSpeed;					//방패없을때 보스의 속도
	bool _isActiveShield;

	vector<bullet> _vBullet;			//불릿
public:
	virtual void attack();
	virtual void move();
	virtual void collide();

	virtual void turn();
	virtual void shieldOff();
	virtual bool stun();
	virtual void moveOff();

	virtual void takeShield();
	virtual void damaged(actor *e);

	virtual void Bfire(float angle);
	virtual void Bmove();
	virtual void Bcollide();
	virtual void Brender();

	void stateUpdate();

	void alphaInit();
	void alphaUpdate();

	virtual HRESULT init(float x, float y, int dir);
	virtual void update();
	virtual void render();
	virtual void release();

	virtual bool frameMake(image * bmp, int & count, int & index, int cooltime);

	virtual void setShieldLink(tortoiseShield* shield) { _shield = shield; }

	tortoisePhase1() {}
	~tortoisePhase1() {}
};