#pragma once
#include "enemy.h"
class tortoiseShield : public enemy
{
	bool on;						//���� ì���?
	float x;						//���� x
	float y;						//���� y
	RECT hitBox;					//���� �浹�ڽ�
	int count;						//���� ������ ī��Ʈ
	int index;						//���� ������ �ε���
	float offSpeed;					//���а� ������ �ӵ�
public:
	HRESULT init();
	void update();
	void render();
	void release();

	tortoiseShield() {}
	~tortoiseShield() {}
};

