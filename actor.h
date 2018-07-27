#pragma once
#include "gameNode.h"
class actor : public gameNode
{
protected:
	float _x, _y;
	float _weight;
	float _gravity;
	float _speed;
	float _power;
	float _angle;

	int _hp;
	int _maxHp;
	int _proveX;		// ��������
	int _proveY;		// ��������

	int _state;
	int _dir;		// ����: ��ȭ�� ���� ���

	int _curFrameX, _curFrameY;
	int _count;			// ������ �ε��� �� ���ϱ�
	int _index;			// ������ ������
	RECT _hitBox;

	bool _isLiftable;

public:		// ��� �Լ�
	virtual void attack() {}
	virtual void damaged(actor *e) {}	// ü�� ����, �˹�, ���� ����Ʈ����(������), ������ �� �ǰݽ� �����(�Ե�, ������)
	virtual void move();
	virtual void collide(){}
	virtual void throwed(float speed, float angle);
	virtual void drawUI() {}

	void setIsLiftable(bool isLiftable) { _isLiftable = isLiftable; }
	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
	void setWeight(float weight) { _weight = weight; }
	void setGravity(float gravity) { _gravity = gravity; }
	void setSpeed(float speed) { _speed = speed; }
	void setPower(float power) { _power = power; }
	void setAngle(float angle) { _angle = angle; }

	void setHp(int hp) { _hp = hp; }
	void setMaxHp(int maxHp) { _maxHp = maxHp; }
	void setProveX(int proveX) { _proveX = proveX; }
	void setProveY(int proveY) { _proveY = proveY; }
	void setState(int state) { _state = state; }
	void setDir(int dir) { _dir = dir; }
	void setRect(RECT hitBox) { _hitBox = hitBox; }

	void setCount(int count) { _count = count; }
	void setIndex(int index) { _index = index; }

	bool getIsLiftable() { return _isLiftable; }
	float getX() { return _x; }
	float getY() { return _y; }
	float getWeight() { return _weight; }
	float getGravity() { return _gravity; }
	float getSpeed() { return _speed; }
	float getPower() { return _power; }
	float getAngle() { return _angle; }
	RECT getHitbox() { return _hitBox; }

	int getHp() { return _hp; }
	int getMaxHp() { return _maxHp; }
	int getState() { return _state; }
	int getProveX() { return _proveX; }
	int getProveY() { return _proveY; }
	int getDir() { return _dir; }
	int getCount() { return _count; }
	int getIndex() { return _index; }
	RECT getRect() { return _hitBox; }

	bool collide(actor* a);

	virtual HRESULT init();
	virtual void update();
	virtual void render();
	virtual void release();
public:
	actor();
	virtual ~actor() {}
};

