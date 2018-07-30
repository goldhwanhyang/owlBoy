#pragma once
#include "enemy.h"
#include "progressBar.h"
#include "bullet.h"
#include "tortoiseShield.h"

namespace PHASE2_CONST
{
	const int MAX_STATE = 7;
	const float DEFAULT_OFF_SPEED = 4.0f;
	//const float MAX_OFF_SPEED = 8.0f;
}

class tortoisePhase2 : public enemy
{
private:
	enum STATE
	{
		READY,

		FLY,
		ATTACK,

		TAKE_SHIELD,

		OFF_SHIELD,
		OFF_STUN,
		OFF_FLY
	};
	enum DIRECTION
	{
		RIGHT,
		LEFT
	};

	bool _isAttack;						//������
	int _attackCount;					//��߽����� üũ
	int _delayCount;					//�Ҹ� ������

	image* _tortoiseImage[PHASE2_CONST::MAX_STATE];	//���� �̹���[����]			
	tortoiseShield* _shield;			//����
	float _offSpeed;					//���о����� ������ �ӵ�
	bool _isActiveShield;

	vector<bullet> _vBullet;			//�Ҹ�

	char _debug[64];
public:
	virtual void attack();
	virtual void move();
	virtual void collide();

	virtual void shieldOff();
	virtual bool stun();
	virtual void moveOff();

	virtual void takeShield();
	virtual void damaged(actor *e);

	virtual void Bfire(float angle);
	virtual void Bmove();
	virtual void Brender();

	virtual HRESULT init(float x, float y, int dir);
	virtual void update();
	virtual void render();
	virtual void release();

	virtual bool frameMake(image * bmp, int & count, int & index, int frameY1, int frameY2, int cooltime, bool renderDir);

	virtual void setShieldLink(tortoiseShield* shield) { _shield = shield; }

	tortoisePhase2() {}
	~tortoisePhase2() {}
};

