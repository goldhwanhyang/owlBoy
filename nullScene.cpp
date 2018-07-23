#include "stdafx.h"
#include "nullScene.h"

HRESULT nullScene::init()
{
	/*
	//���Ӿ� �ʱ�ȭ
	����ȭ�� Ŭ���� ������ �ʱ�ȭ
	_startScene = new aaa;
	_startScene->init();
	//����ȭ�� Ŭ���� ������ �ʱ�ȭ
	_gameScene = new bbb;
	_gameScene->init();
	//����� �ʱ�ȭ
	//_currentScene = _startScene;
	*/
	//��׶��� �̹��� �ʱ�ȭ
	//_bgImage = new image;
	//_bgImage->init("Texture/background.bmp", WINSIZEX, WINSIZEY);
	IGM->addImage("���", "Texture/background.bmp", WINSIZEX, WINSIZEY);
	IGM->addFrameImage("�÷��̾�", "Texture/aladdin.bmp",float(WINSIZEX/2),float(WINSIZEY/2), 763, 124, 13, 2); //x,y�� �Ǽ������� ����Ұ�
	//������ �̹��� �ʱ�ȭ

	//���ʹ̸Ŵ��� Ŭ���� ���� �� �ʱ�Ȯ
	/*
	_enemyManager = new enemyManager;
	_enemyManager->init();
	*/

	//�簢�� ��ġ �ʱ�ȭ

	CAM->setPosition(WINSIZEX/2, WINSIZEY/2); //ī�޶� �ʱ���ǥ

	return S_OK;
}

void nullScene::release()
{
	//�̹��� Ŭ���� ���������� ����� �� ����
	//init�Լ����� �����Ҵ� new�� ����ߴٸ� �̰����� delete ����Ѵ�

	/*
	//����ȭ�� ����ȭ�� ����
	_startScene->release();
	SAFE_DELETE(_startScene);
	_gameScene->release();
	SAFE_DELETE(_gameScene);
	*/

	//���ʹ̸Ŵ��� ����
	/*
	_enemyManager->release();
	SAFE_DELETE(_enemyManager);
	*/
}

void nullScene::update()
{
	//�̰����� ����, Ű����, ���콺��� ������Ʈ�� �Ѵ�
	//�����ϰ� �̰����� �ڵ��Ѵٰ� �����ϸ� �ȴ�

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		CAM->setShakeInfo(20, 30);
	}

	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		EFFECT->generateEffect("�÷��̾�", _ptMouse.x, _ptMouse.y, 0);
	}

	/*
	//������� ������Ʈ
	_currentScene->update();

	if (KEYMANAGER->isOnceKeyDown('1'))
	{
	_currentScene = _startScene;
	_currentScene->init();
	}
	if (KEYMANAGER->isOnceKeyDown('2'))
	{
	_currentScene = _gameScene;
	_currentScene->init();
	}*/

	//���Ŵ��� Ŭ���� ������Ʈ
	/*
	_enemyManager->update();
	*/
	CAM->videoShooting();	// ������ ������ x, y ��ǥ�� �Կ�
	//CAM->videoShooting(player.x, player.y, player.speed);	// �÷��̾��� x, y��ǥ�� ����, speed�� 0�̸� �÷��̾� ��ġ�� ���� �Ѿư�. speed�� 0�� �ƴϸ� speed �ӵ��� �Ѿư�
}

void nullScene::render()
{
	//�̰����� WM_PAINT���� �ߴ����� ó���ϸ� �ȴ�

	//��׶��� �̹��� ����
	//���1 -> �̹���Ŭ����Ȱ��
	//_bgImage->render(getMemDC());
	//���2 -> �̹����Ŵ���,Ű��
	IGM->render("bgLoadingScene", getMemDC(), CAM->getSX(), CAM->getSY());
	IGM->frameRender("�÷��̾�", getMemDC(), IGM->findI("�÷��̾�")->getX() + CAM->getSX(), IGM->findI("�÷��̾�")->getY() + CAM->getSY());

	//���3 -> �̹����Ŵ���,find (��������)
	//IMAGEMANAGER->findImage("���)->render(getMemDC());

	// ĳ���� ������ �̹��� ����
	//_player->frameRender(memDC, _aladdin.bmp->getX(), _aladdin.bmp->getY());

	/*
	//������� ����
	_currentScene->render();
	*/

	//���ʹ̸Ŵ��� Ŭ���� ����
	/*
	_enemyManager->render();
	*/
	EFFECT->render(getMemDC());
}

void nullScene::collision()
{
	/*
	//�浹ó��
	//�÷��̾��� �Ѿ� & �� => �浹ó��
	//�÷��̾�� �Ѿ�Ŭ���� �����´�
	//�Ѿ�Ŭ�������� ���� �����´�
	//�÷��̾�->�̻���Ŭ����->�Ѿ˺���[]
	for (int i = 0; i < _player->getMissile()->getVBullet().size(); i++)
	{
	//RECT rc;//�ӽÿ뷺Ʈ
	//RECT rcEnemy;
	//RECT rcBullet = _player->getMissile()->getVBullet()[i].rc;
	//if (IntersectRect(&rc, &rcBullet, &rcEnemy))
	//{
	//
	//}
	}

	//���ʹ�Ŭ������ �Ѿ�Ŭ������ ������
	���ʹ̸Ŵ���->���ʹ�Ŭ����->�Ѿ�Ŭ����->�Ѿ˺���

	//���ʹ̸Ŵ������� �Ѿ�Ŭ���� ��� ������
	���ʹ̸Ŵ���->�Ѿ�Ŭ����->�Ѿ˺���

	*/
}
