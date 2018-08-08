#pragma once
#include "actor.h"				// ����� �θ�Ŭ����

class enemyManager;	// enemyManager��� Ŭ������ �ִٴ°��� �˸��� ���� ���漱���� ���־���.

#define OTUS_WIDTH 50
#define OTUS_HEIGTH 125
#define PI 3.14
#define STATE_MAX 11	// ���� �߰� �ɶ����� ��������� ���� �÷��ֱ�
enum STATE { IDLE , WALK, JUMP, JUMPFALL , FLY , FLYDOWN, ROLL , ATK, LIFT, LIFT2, HIT };
enum WAY { RIGHT , LEFT, UP, DOWN, NONE};	// ����, ������ , �� , �Ʒ� , �ƹ��͵� ������ �ʾ��� ��
enum FLYING { FLY_L, FLY_R, FLY_U, FLY_D, FLY_N };

//�Ұ����δ� 
class player : public actor
{
private:
	enemyManager * _enemyManager;
	/*
		������ : Ʋ�� ���ؼ� �ٸ����� ������� �ִ� ���ʹ̸Ŵ����� ����ϰڴ�. ( ���ʹ̸Ŵ����� setPlayer(_player) ���� ������ �����ͼ� Ʋ�� ��´� )
		���ŷӰ� mainGame�̳� Scene�� ��ġ�� �ʰ� �����ϰ� ���� ���� ���� �����ϰ� ���ش�.	
	*/


	int _beforeState;
	WAY _axisX, _axisY;	// �Ű�� ���ȴ��� , �ƹ��͵� �ȴ��ȴ��� Ȯ��
	FLYING _FX, _FY;	//
	image* img[STATE_MAX];
	image* _mapPixel;
	image* _map;

	image* friendsFace;
	image* hpBarBack;
	image* hpBarFront;

	RECT _spinHitBox;
	bool _isCol;
	bool _isLeft;
	bool _isFly;	// �����ִ� �������� �ƴ���
	int _jumpCount; // �����ִ��� �����ִ��� �����ϱ����� ����


	float _jumpSpeed;	// ���� ���ǵ� �߷�
	float _flySpeed;	// �������� �� �ӵ�
	float _rollSpeed;	// ������ �ӵ�


	int _oldX;	// ���� x��ġ
	int _oldY;	// ���� y��ġ
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
	void collideMap();
	void collideActor();

	// �÷��̾ ������� ������ �ڱ� �ڽ��� �Ǹ� ��´�
	virtual void damaged(actor *e) {}	// ü�� ����, �˹�, ���� ����Ʈ����(������), ������ �� �ǰݽ� �����(�Ե�, ������)


	void frameSetting();


	void setMap(image *map) { _map = map; }
	void setMapPixel(image *mapPixel) { _mapPixel = mapPixel; }
	void setEnemyManager(enemyManager *e) { _enemyManager = e; }	// Ʋ�� �ִ� _enemyManager�� �ٸ������ִ� ���ʹ̸Ŵ����������� ���������� �Լ�.

	// ���� ) �ſ� ���� �������� �������θ� ���� �ȵ������ϱ� ���̶�� ������ ����ؼ� ���� ������
	// �� = Ʋ , �� = �˸���, �� = set�Լ�
	player(){}
	~player(){}
};