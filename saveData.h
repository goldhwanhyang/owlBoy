#pragma once
#include "singletonBase.h"

//씬 전환등에 사용하도록 하자

class saveData : public singletonBase <saveData>
{
private:
	HANDLE _saveHandle;
	DWORD _result;
	float _hp;
	char _data[128];

public:
	HRESULT init();
	void release();

	int getHp() { return _hp; }
	void setHp(int hp) { _hp = hp; }

	saveData() {}
	~saveData() {}
};


//예시
//체력 저장
//if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
//{
//	SAVEDATA->setHp(_currentHp);
//}
////체력 로드
//if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
//{
//	_currentHp = SAVEDATA->getHp();
//}

