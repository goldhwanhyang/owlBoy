#pragma once
#include "singletonBase.h"

class backgroundActor;
class actor;
class RenderManager : public singletonBase<RenderManager>
{
private:
	multimap<float, actor *> renderList;
	multimap<float, backgroundActor *> backgroundList;

public:
	HRESULT init();	// void�� ����ص� ��. HRESULT: �������� ���� ���� ���� ���� ���� ���� �� ����. ���� ���� ����ϴ� ���� S_OK(���� ����), E_FAIL(���� �߻�)
	void release();
	void render(HDC hdc);
	void backgroundRender(HDC hdc);

	void addRender(float z, actor *name);
	void addBackground(float z, backgroundActor *backActor);
	void clearBackground();


	RenderManager() {}
	~RenderManager() {}
};

