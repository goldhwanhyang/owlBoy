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
	enemy * createEnemy(int type, float x = WINSIZEX/2, float y = WINSIZEY/2, int dir = 1);
};

//���ʹ� �Ŵ����� �ٽ��� ��� ���ʹ̸� ���Ϳ� ��� �� ���͸� ��ȯ���ִ°Ͱ�, �ʱ�ȭ,������Ʈ,���� ���ֱ�
class enemyManager : public gameNode
{
private:
	player* _player;
	image* _mapPixel;
	vector<enemy*> _vEnemy;
public:
	virtual HRESULT init();
	virtual void update();
	virtual void render();
	virtual void release();

	void setPlayer(player *p);// { _player = p; }
	void setMapPixel(image* map);// { _mapPixel = map; }
	vector<enemy*> getVEnemy() { return _vEnemy; }

	void addEnemy(enemy* enemy) { _vEnemy.push_back(enemy); }
	void setPostion(int index, float x, float y, int dir = 1) { _vEnemy[index]->setPostion(x, y, dir); }

	enemyManager() {}
	~enemyManager() {}
};