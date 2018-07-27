#pragma once
#include "enemy.h"
#include "bullet.h"
#define MAX_STATE 7

class boss1 : public enemy
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
		OFF_WALK
	};
	enum DIRECTION
	{
		RIGHT,
		LEFT
	};
	float _speed2;
	bool _isShield;
	RECT _shieldHitBox;
	bool _isAttack;
	int _attackCount;
	int _delayCount;

	image* _boss1Image[MAX_STATE];
	image* _shieldDropImage;

	vector<bullet> _vBullet;

	char _debug[128];
public:
	virtual void attack();
	virtual void move();
	virtual void turn();

	void Bfire(float angle);
	void Bmove();
	void Brender();

	virtual HRESULT init(float x, float y);
	virtual void update();
	virtual void render();
	virtual void release();

	bool frameMake(image * bmp, int & count, int & index, int frameY1, int frameY2, int cooltime, bool renderDir);

	boss1() {}
	~boss1() {}
};

