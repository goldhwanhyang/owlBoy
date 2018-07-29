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
	HRESULT init();	// void를 사용해도 됨. HRESULT: 정상적인 종료 여부 등의 여러 값을 가질 수 있음. 제일 많이 사용하는 것이 S_OK(정상 종료), E_FAIL(에러 발생)
	void release();
	void render(HDC hdc);
	void backgroundRender(HDC hdc);

	void addRender(float z, actor *name);
	void addBackground(float z, backgroundActor *backActor);
	void clearBackground();


	RenderManager() {}
	~RenderManager() {}
};

