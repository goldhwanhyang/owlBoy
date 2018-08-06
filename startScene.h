#pragma once
#include "gameNode.h"
#include "progressBar.h"

class startScene :
	public gameNode
{
	image *_bgImage, *_bgCloud, *_stone1, *_stone2;
	image *_title;
	image *_text[3];
	
	image *_board, *_backToMenu;

	progressBar * _volume;

	int _selectedAlpha, _nonAlpha;
	int _count, _index;

	int _curMenu;

	int _selectedOption;
	int _maxOption;

	int _volumeWidth, _volumeX, _volumeY;

	float _angle;
public:
	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	void updateStartMenu();
	void updateOption();
	void drawOption();
	void drawStartMenu();

	startScene() {}
	~startScene() {}
};

