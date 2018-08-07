#pragma once
#include "gameNode.h"
#include "enemy.h"
#include "tortoise.h"
#include "gawk.h"
#include "torque.h"

class player;	// player��� Ŭ������ �ִٴ°� �˸��� ���� ���漱���� ���־���.

enum ENEMY_TYPE
{
	GAWK,
	TORQUE
};

class enemyFactory
{
public:
	enemy * createEnemy(int type);
};

//���ʹ� �Ŵ����� �ٽ��� ��� ���ʹ̸� ���Ϳ� ��� �� ���͸� ��ȯ���ִ°Ͱ�, �ʱ�ȭ,������Ʈ,���� ���ֱ�
class enemyManager : public gameNode
{
private:
	player* _player;
	image* _mapPixel;

	enemyFactory* _factory;
	vector<enemy*> _vEnemy;
	tortoiseShield* _shield;
public:
	virtual HRESULT init();
	virtual void update();
	virtual void render();
	virtual void release();

	void initTortoise();
	void setPlayer(player *p) { _player = p; }
	void setMapPixel(image* map) { _mapPixel = map; }
	vector<enemy*> getVEnemy() { return _vEnemy; }
	tortoiseShield* getTortoiseShield() { return _shield; }

	enemyManager() {}
	~enemyManager() {}
};