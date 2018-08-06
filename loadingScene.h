#pragma once
#include "gameNode.h"
#include "loading.h"

class loadingScene : public gameNode
{
private:
	loading * _loading;

	image * _otus, *_otusOver;
	image * _line;
	image * _lineBottom;


	int _count, _index;
	float _height;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	//�ε��̹��� �Լ�
	void loadingImage();
	//�ε����� �Լ�
	void loadingSound();
	//�ε�����Ʈ �Լ�
	void loadingEffect();

	loadingScene() {}
	~loadingScene() {}
};

