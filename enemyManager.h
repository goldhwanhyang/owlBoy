#pragma once
#include "gameNode.h"
#include "player.h"
#include "enemy.h"
#include "tortoise.h"
#include "gawk.h"
#include "torque.h"

//에너미 매니저의 핵심은 모든 에너미를 벡터에 담고 그 벡터를 반환해주는것과, 초기화,업데이트,렌더 해주기
class enemyManager : public gameNode
{
private:
	tortoiseShield* _shield;
	vector<enemy*> _vEnemy;
	player* _player;
public:
	virtual HRESULT init();
	virtual void update();
	virtual void render();
	virtual void release();

	void setPlayer(player *p) { _player = p; }
	vector<enemy*> getVEnemy() { return _vEnemy; }
	tortoiseShield* getTortoiseShield() { return _shield; }

	enemyManager() {}
	~enemyManager() {}
};