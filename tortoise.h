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

	bool _isAttack;						//������
	int _attackCount;					//��߽����� üũ
	int _delayCount;					//�Ҹ� ������

	image* _tortoiseImage[MAX_STATE];	//���� �̹���[����]			
	tortoiseShield* _shield;			//����
	float _offSpeed;					//���о����� ������ �ӵ�

	vector<bullet> _vBullet;			//�Ҹ�

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

