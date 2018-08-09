#pragma once
#include "singletonBase.h"
#include "image.h"

class progressBar;

class UIManager :
	public singletonBase<UIManager>
{
private:
	image * _magenta;
	image * _uiDC;
	
	int _uiType;

	bool _isBlockingUI;
	bool _isDrawUI;


	/////// volume UI
	image *_board, *_backToMenu;
	progressBar * _volume;
	int _volumeWidth, _volumeX, _volumeY;

public:
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	bool checkBlocking() { return _isBlockingUI; }
	bool checkDrawingUI() { return _isDrawUI; }
	void clear();
	void drawVolumeSetting();
	HDC getUIDC() { return _uiDC->getMemDC(); }


	UIManager() : _isBlockingUI(false), _isDrawUI(false) {}
	~UIManager() {}
};

