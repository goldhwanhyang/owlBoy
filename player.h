#pragma once
#include "actor.h"				// ����� �θ�Ŭ����
#include "geddy.h"
// liftableActor
// stuffMAnager��ȣ����

class enemyManager;	// enemyManager��� Ŭ������ �ִٴ°��� �˸��� ���� ���漱���� ���־���.
class stuffManager;	// stuffManager��� Ŭ������ �ִٴ°��� �˸��� ���� ���漱���� ���־���.

#define OTUS_WIDTH 50
#define OTUS_HEIGTH 125
#define PI 3.14
#define STATE_MAX 10	// ���� �߰� �ɶ����� ��������� ���� �÷��ֱ�
enum STATE { IDLE , WALK, JUMP, JUMPFALL , FLY , FLYDOWN, ROLL , ATK, LIFT,HIT };
enum WAY { RIGHT , LEFT, UP, DOWN, NONE};	// ����, ������ , �� , �Ʒ� , �ƹ��͵� ������ �ʾ��� ��
enum FLYING { FLY_L, FLY_R, FLY_U, FLY_D, FLY_N };

//�Ұ����δ� 
class player : public actor
{
private:
	enemyManager* _enemyManager;
	stuffManager* _stuffManager;

	/* 
		������ : Ʋ�� ���ؼ� �ٸ����� ������� �ִ� ���ʹ̸Ŵ����� ����ϰڴ�. ( ���ʹ̸Ŵ����� setPlayer(_player) ���� ������ �����ͼ� Ʋ�� ��´� )
		���ŷӰ� mainGame�̳� Scene�� ��ġ�� �ʰ� �����ϰ� ���� ���� ���� �����ϰ� ���ش�.	
	*/
	liftableActor* _liftableActor;
	geddy* _geddy;

	RECT _progressBarRc;

	int _beforeState;
	WAY _axisX, _axisY;	// �Ű�� ���ȴ��� , �ƹ��͵� �ȴ��ȴ��� Ȯ��
	FLYING _FX, _FY;	//
	image* img[STATE_MAX];
	image* _liftImg;
	image* _mapPixel;
	image* _map;

	image* friendsFace;
	image* hpBarBack;
	image* hpBarFront;

	RECT _spinHitBox;
	RECT _rollHitBox;
	bool _isKnockBack;
	bool _isBack;
	bool _isLeft;
	bool _isFly;	// �����ִ� �������� �ƴ���
	int _jumpCount; // �����ִ��� �����ִ��� �����ϱ����� ����


	float _jumpSpeed;	// ���� ���ǵ� �߷�
	float _flySpeed;	// �������� �� �ӵ�
	float _rollSpeed;	// ������ �ӵ�
	float _knockBackSpeed;
	float _backSpeed;

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

	void commonInputKey();
	void collide();
	void collideMap();
	void collideActor();
	void collideStuff();

	// �÷��̾ ������� ������ �ڱ� �ڽ��� �Ǹ� ��´�
	virtual void damaged(actor *e) {}	// ü�� ����, �˹�, ���� ����Ʈ����(������), ������ �� �ǰݽ� �����(�Ե�, ������)

	void frameSetting();

	void setMap(image *map) { _map = map; }
	void setMapPixel(image *mapPixel) { _mapPixel = mapPixel; }
	void setEnemyManager(enemyManager* e) { _enemyManager = e; }	// Ʋ�� �ִ� _enemyManager�� �ٸ������ִ� ���ʹ̸Ŵ����������� ������������ �Լ�.
	void setStuffManager(stuffManager* s) { _stuffManager = s; }	// Ʋ�� �ִ� _stuffManager�� �ٸ������ִ� �������Ŵ����������� ������������ �Լ�.
	void setliftableActor(liftableActor* l) { _liftableActor = l; }
	void setGeddy(geddy *g) { _geddy = g; }

	// ���� ) �ſ� ���� �������� �������θ� ���� �ȵ������ϱ� ���̶�� ������ ����ؼ� ���� ������
	// �� = Ʋ , �� = �˸���, �� = set�Լ�
	player():_stuffManager(NULL), _liftableActor(NULL), _geddy(NULL){}
	~player(){}
};