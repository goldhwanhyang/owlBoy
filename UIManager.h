#pragma once
#include "singletonBase.h"
class UIManager :
	public singletonBase<UIManager>
{
public:
	HRESULT init(void);
	void release(void);
	void update(float lockFPS);
	void render(HDC hdc);

	UIManager() {}
	~UIManager() {}
};

