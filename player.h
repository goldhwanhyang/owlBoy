#pragma once
#include "actor.h"

#define OTUS_WIDTH 30
#define OTUS_HEIGTH 50


class player : public actor
{
private:
	image* img;
	image* _testMap;

	bool isDrop;
public:
	HRESULT init();
	void update();
	void render();
	void release();

	player(){}
	~player(){}
};