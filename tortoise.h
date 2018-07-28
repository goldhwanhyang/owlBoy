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
		bool on;						//���� ì���?
		float x;						//���� x
		float y;						//���� y
		RECT hitBox;					//���� �浹�ڽ�
		int count;						//���� ������ ī��Ʈ
		int index;						//���� ������ �ε���
		float offSpeed;					//���а� ������ �ӵ�
	};

	bool _isAttack;						//������
	int _attackCount;					//��߽����� üũ
	int _delayCount;					//�Ҹ� ������

	image* _tortoiseImage[MAX_STATE];	//���� �̹���[����]
	image* _shieldDropImage;			//������ ���� �̹���
	Shield _shield;						//������ ������

	vector<bullet> _vBullet;			//�Ҹ�

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

