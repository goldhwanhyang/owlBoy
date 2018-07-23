#pragma once
#include "gameNode.h"
#include "minion.h"
#include "bullet.h"


class enemyManager : public gameNode
{
private:
	vector<enemy*> _vMinion;
	vector<enemy*>::iterator _viMinion;

	bullet* _bullet;	//공용총알 클래스
	int _count;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//미니언 세팅 => 초기화에 넣기
	void setMinion();

	//미니언 총알발사
	void minionBulletFire();

	//미니언 삭제
	void removeMinion(int index);

	//미니언 벡터 가져오기
	vector<enemy*> getMinion() { return _vMinion; }

	enemyManager() {}
	~enemyManager() {}
};

