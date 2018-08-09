#pragma once
#include "liftableActor.h"
#include "bullet.h"

class player;
class enemyManager;

#define MAX_GEDDY_STATE 6
#define MAX_GEDDY_BULLET 20

#define GEDDY_WIDTH 50
#define GEDDY_HEIGHT 150
namespace geddyEnum
{
	enum { IDLE, ON_AIR, HANG, ATTACK, WALK, PREFARE };
}
class geddy : public liftableActor
{
	enum { IDLE, ON_AIR, HANG, ATTACK, WALK, PREFARE };

private:
	image * _img[MAX_GEDDY_STATE];
	image * _handImg;
	//image * _bulletImage;

	bullet *_bullet;

	enemyManager * _enemyManager;

	int _handsDir;
	int _shootingDir;
public:

	void setEnemyManager(enemyManager * em) { _enemyManager = em; }

	virtual void attack();
	virtual void damaged(actor *e);	// ü�� ����, �˹�, ���� ����Ʈ����(������), ������ �� �ǰݽ� �����(�Ե�, ������)
	virtual void move();
	virtual void collide();
	void collideWall();
	void collideEnemy();
	//virtual void throwed(float speed, float angle);
	virtual void lifted(player* _player);
	virtual void drawUI();

	virtual HRESULT init();
	virtual void update();
	virtual void render();
	virtual void release();

	int convertDir();

	geddy():_enemyManager(NULL){}
	~geddy() {}
};

