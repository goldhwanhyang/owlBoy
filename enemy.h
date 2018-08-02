#pragma once
#include "actor.h"
class player;
class enemy : public actor
{
protected:
	player* _player;
	float _playerX, _playerY;
public:
	virtual HRESULT init(float x = WINSIZEX / 2, float y = WINSIZEY / 2, int dir = 1);
	virtual void update();
	virtual void render();
	virtual void release();

	void setPostion(float x, float y, int dir = 1) { _x = x; _y = y; _dir = dir; }
	void setPlayerLink(player* player) { _player = player; }
	
	enemy() {}
	~enemy() {}
};

