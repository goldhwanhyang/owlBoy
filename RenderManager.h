#pragma once
#include "singletonBase.h"

class gameNode;

class RenderManager : public singletonBase<RenderManager>
{
private:
	multimap<float, gameNode *> renderList;

public:
	HRESULT init();	// void�� ����ص� ��. HRESULT: �������� ���� ���� ���� ���� ���� ���� �� ����. ���� ���� ����ϴ� ���� S_OK(���� ����), E_FAIL(���� �߻�)
	void release();
	void render(HDC hdc);

	void addRender(gameNode *effectName, float z);

	RenderManager() {}
	~RenderManager() {}
};

