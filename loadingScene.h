#pragma once
#include "gameNode.h"
#include "loading.h"

class loadingScene : public gameNode
{
private:
	loading * _loading;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//로딩이미지 함수
	void loadingImage();
	//로딩사운드 함수
	void loadingSound();
	//로딩이펙트 함수
	void loadingEffect();

	loadingScene() {}
	~loadingScene() {}
};

