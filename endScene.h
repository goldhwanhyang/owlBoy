#pragma once
#include "gameNode.h"
class endScene :
	public gameNode
{

	int _y, _count;

public:
	virtual HRESULT init();				//void�� �ᵵ�Ǵµ� �ʱ�ȭ�� ������ ����� �ٷ� �˷���
	virtual void release();
	virtual void update();
	virtual void render();

	endScene() {}
	~endScene() {}
};

