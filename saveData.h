#pragma once
#include "singletonBase.h"
//�� ��ȯ� ����ϵ��� ����

class player;
class geddy;

class saveData : public singletonBase <saveData>
{
private:
	HANDLE _saveHandle;
	DWORD _result;
	float _hp;
	char _data[128];

	player* _player;
	geddy* _geddy;

public:
	HRESULT init();
	void release();

	int getHp() { return _hp; }
	void setHp(int hp) { _hp = hp; }

	player* getPlayer() { return _player; }
	void setPlayer(player * p) { _player = p; }

	geddy* getGeddy() { return _geddy; }
	void setGeddy(geddy * g) { _geddy = g; }

	saveData() {}
	~saveData() {}
};


//����
//ü�� ����
//if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
//{
//	SAVEDATA->setHp(_currentHp);
//}
////ü�� �ε�
//if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
//{
//	_currentHp = SAVEDATA->getHp();
//}

