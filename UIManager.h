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

	HBRUSH brush;

	////// flickering
	
	int _alpha;
	int _count;
	int _speed;
	COLORREF _color;

	////// sceneChange

	bool _sceneChanging;
	bool _startingScene;
	bool _endScene;
	int _destX, _destY;
	int _rcWidth, _rcHeight;

	/////// volume UI
	image *_board, *_backToMenu;
	progressBar * _volume;
	int _volumeWidth, _volumeX, _volumeY;

public:
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc);

	void flickering(COLORREF color, int speed, int count);
	void sceneChange(HDC hdc);
	void startingSceneChange(int x, int y);

	void newSceneStart(HDC hdc);
	void startingNewScene(int x, int y);

	bool checkEndScene() { return _endScene; }
	bool checkBlocking() { return _isBlockingUI; }
	bool checkDrawingUI() { return _isDrawUI; }
	void clear();
	void drawVolumeSetting();
	HDC getUIDC() { return _uiDC->getMemDC(); }
	bool isChangingScene() { return (_sceneChanging || _startingScene); }


	UIManager() : _isBlockingUI(false), _isDrawUI(false), _endScene(false) {}
	~UIManager() {}
};

