#pragma once
#include "gameNode.h"
#include "player.h"
#include "effect.h"
#include "ironWeight.h"
#include "cloud.h"
#include "stone.h"
#include "geddy.h"
#include "ring.h"
#include "stuffManager.h"

#define MAX_FRAME_3 20
#define MAX_FRAME_10 7
#define MAX_RING 20

class townScene : public gameNode
{
private:
	//image* _testMap;
	image * _backgroundSky;
	image * _topCloud, *_bottomCloud;

	image * _TownMap;
	image* _TownMapPixel;
	player* _player;
	RECT portal;

	geddy * _geddy;

	int tempCount = 0;

	bool endScene;

	stuffManager * _stuffManager;
	vector<ring *> _vRing;

	struct backObject
	{
		image* _img;
		POINT _pos;
	};
	backObject _backgroundObject[MAX_FRAME_3];
	backObject _bush[MAX_FRAME_10];

	int _bushFrameX, _bushFrameY, _bushFrameX2, _bushFrameY2, _bushCount;
public:

	HRESULT init();
	void release();
	void update();
	void render();

	townScene() {}
	~townScene() {}
};

