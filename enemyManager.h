#pragma once
#include "gameNode.h"
#include "enemy.h"
#include "tortoise.h"
#include "gawk.h"
#include "torque.h"

class player;	// player라는 클래스가 있다는걸 알리기 위해 전방선언을 해주었다.

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

//에너미 매니저의 핵심은 모든 에너미를 벡터에 담고 그 벡터를 반환해주는것과, 초기화,업데이트,렌더 해주기
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