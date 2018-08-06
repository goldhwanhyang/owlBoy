#pragma once
#include "gameNode.h"
#include "liftableActor.h"
#include "fruit.h"
#include "ironWeight.h"

class player;

class stuffManager : public gameNode
{
private:
	vector<liftableActor *> _vStuff;
	vector<liftableActor *> _vFruit;

	player * _player;

public:
	HRESULT init();
	void release();
	void update();
	void render();
	liftableActor* collide(player * _player);

	vector<liftableActor *>& getVStuff() { return _vStuff; }

	void setPlayer(player * p) { _player = p; }
	void addFruit(liftableActor *fruit) { _vFruit.push_back(fruit); }
	void addStuff(liftableActor *stuff) { _vStuff.push_back(stuff); }
	void addFruit(int type, float x, float y);
	void addStuff(int type, float x, float y);
	
	void removeFruit(int index);

	stuffManager() {}
	~stuffManager() {}
};

