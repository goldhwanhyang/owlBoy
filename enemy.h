#pragma once
#include "actor.h"
class player;
class enemy : public actor
{
protected:
	player* _player;
	float _playerX, _playerY;
public:
	virtual HRESULT init(float x, float y);
	virtual void update();
	virtual void render();
	virtual void release();

	void setPlayerLink(player* player) { _player = player; }

	enemy() {}
	~enemy() {}
};

