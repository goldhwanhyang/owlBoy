#pragma once
#include "enemy.h"
#include "bullet.h"

namespace TORQUE_CONST
{
	const int MAX_STATE = 5;
	const int HITBOX_WIDTH = 90;
	const int HITBOX_HEIGHT = 100;
}

class torque : public enemy
{
	image* _torqueImage[TORQUE_CONST::MAX_STATE];
	image* _torqueHand[TORQUE_CONST::MAX_STATE];
	enum STATE
	{
		IDLE,
		READY,
		AIMING,
		SHOOT,
		STUN
	};
	enum DIR
	{
		RIGHT,
		LEFT
	};
	RECT _scanRc;

	int _readyCount;			//�غ� ������
	int _aimingCount;			//���� ������
	int _stunCount;				//���� ������
	int _attackCount;			//���� ������

	vector<bullet> _vBullet;	//�Ҹ�

public:
	virtual void damaged(actor* e);
	virtual void collide();

	void search();
	void turn();

	virtual void Bfire(float angle);
	virtual void Bmove();
	virtual void Bcollide();
	virtual void Brender();

	virtual HRESULT init(float x, float y, int dir);
	virtual void update();
	virtual void render();
	virtual void release();

	bool frameMake(image * bmp);

	torque() {}
	~torque() {}
};

