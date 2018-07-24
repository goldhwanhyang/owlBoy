#pragma once
#include "singletonBase.h"
#include "image.h"

struct MyEffect
{
	image * eImage;
	float x, y;
	int index, count, frameY;
};

class MyEffectManager : public singletonBase<MyEffectManager>
{
private:
	list<MyEffect> effectList;

public:
	HRESULT init();	// void�� ����ص� ��. HRESULT: �������� ���� ���� ���� ���� ���� ���� �� ����. ���� ���� ����ϴ� ���� S_OK(���� ����), E_FAIL(���� �߻�)
	void release();
	void render(HDC hdc);

	void generateEffect(const char *effectName, float x, float y, int frameY = 0);

	MyEffectManager();
	~MyEffectManager();
};

