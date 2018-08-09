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

	bool _isAttack;						//������
	int _attackCount;					//��߽����� üũ
	int _delayCount;					//�Ҹ� ������

	image* _tortoiseImage[PHASE1_CONST::MAX_STATE];	//���� �̹���[����]
	image* _tortoiseDamagedImg[PHASE1_CONST::MAX_STATE]; //���� �ǰ� �̹���
	int _damAlpha;
	int _damAlphaBlue;
	int _alphaDelay;
	int _alphaCount;
	bool _isAlpha;

	tortoiseShield* _shield;			//����

	float _offSpeed;					//���о����� ������ �ӵ�
	bool _isActiveShield;

	vector<bullet> _vBullet;			//�Ҹ�
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