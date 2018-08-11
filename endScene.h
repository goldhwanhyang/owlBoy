#pragma once
#include "gameNode.h"
class endScene :
	public gameNode
{

	int _y, _count, _index[2];
	image* _leftFrame[10];
	image* _rightFrame[10];
	int _currentNum[2];

	image* _backGround;

public:
	virtual HRESULT init();				//void�� �ᵵ�Ǵµ� �ʱ�ȭ�� ������ ����� �ٷ� �˷���
	virtual void release();
	virtual void update();
	virtual void render();

	void creditsText();
	void creditsAni(image** ani, int x, int index);

	endScene() {}
	~endScene() {}
};

