#pragma once
#include "gameNode.h"
#include "player.h"

class storeScene :
	public gameNode
{
	player * _player;
	
public:
	virtual HRESULT init();	
	virtual void release();
	virtual void update();
	virtual void render();

	void setPlayer(player *p) { _player = p; }

	storeScene();
	~storeScene();
};

