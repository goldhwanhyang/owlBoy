#pragma once
#include "actor.h"

#define OTUS_WIDTH 50
#define OTUS_HEIGTH 125
#define PI 3.14
#define STATE_MAX 10	// ���� �߰� �ɶ����� ��������� ���� �÷��ֱ�
enum STATE { IDLE , WALK, JUMP, JUMPFALL , FLY , FLYDOWN, ROLL , ATK, LIFT, LIFT2 };
enum WAY { RIGHT , LEFT, UP, DOWN, NONE};	// ����, ������ , �� , �Ʒ� , �ƹ��͵� ������ �ʾ��� ��
enum FLYING { FLY_L, FLY_R, FLY_U, FLY_D, FLY_N };

//�Ұ����δ� 
class player : public actor
{
private:
	int _beforeState;
	WAY _axisX, _axisY;	// �Ű�� ���ȴ��� , �ƹ��͵� �ȴ��ȴ��� Ȯ��
	FLYING _FX, _FY;	//
	image* img[STATE_MAX];
	image* _mapPixel;
	image* _map;

	bool _isLeft;
	bool _isFly;	// �����ִ� �������� �ƴ���
	int _jumpCount; // �����ִ��� �����ִ��� �����ϱ����� ����


	float _jumpSpeed;	// ���� ���ǵ� �߷�
	float _flySpeed;	// �������� �� �ӵ�
	float _rollSpeed;	// ������ �ӵ�

	int _oldY;	// ���� ��ġ
public:
	HRESULT init();
	void release();
	void update();
	void render();

	//���� ���� ��
	void groundInputKey();							// ���� ���� �� Ű �Է� ( � Ű�� ������ �Ű�� ���ȴ��� Ȯ�����ֱ� )
	void groundAxis(WAY axisX, WAY axisY);			// ���� Ű�� ����� � Ű�� ���ȴ��� Ȯ���ϰ� �����ֱ�
	//void groundMove();								// Axis�� �ٲ� ���¿� ���� �������ֱ�

	//���� ���� ��
	void flyInputKey();
	float flyAngle(FLYING FX, FLYING FY);
	void flyMove();

	//���°� �ٲ𶧸��� index�� 0���� �ʱ�ȭ���ִ� �Լ�
	void changeState(int state);



	void collide();
	void frameSetting();


	void setMap(image *map) { _map = map; }
	void setMapPixel(image *mapPixel) { _mapPixel = mapPixel; }
	player(){}
	~player(){}
};