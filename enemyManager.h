#pragma once
#include "gameNode.h"
#include "player.h"
#include "enemy.h"
#include "tortoise.h"
#include "gawk.h"
#include "torque.h"

//���ʹ� �Ŵ����� �ٽ��� ��� ���ʹ̸� ���Ϳ� ��� �� ���͸� ��ȯ���ִ°Ͱ�, �ʱ�ȭ,������Ʈ,���� ���ֱ�
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