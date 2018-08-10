#pragma once
#include "gameNode.h"
class endScene :
	public gameNode
{
public:
	virtual HRESULT init();				//void로 써도되는데 초기화에 문제가 생기면 바로 알려줌
	virtual void release();
	virtual void update();
	virtual void render();

	endScene() {}
	~endScene() {}
};

