#pragma once
#include "singletonBase.h"
class UIManager :
	public singletonBase<UIManager>
{
private:
	image * _magenta;
	image * _uiDC;
	
	bool _blockingUI;

public:
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	bool checkBlocking() { return _blockingUI; }
	void clear();
	HDC getUIDC() { return _uiDC->getMemDC(); }


	UIManager() : _blockingUI(false) {}
	~UIManager() {}
};

