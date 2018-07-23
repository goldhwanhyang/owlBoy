#include "stdafx.h"
#include "loading.h"
//=============================================================
//	## loadItem ## (�ε������ Ŭ����)
//=============================================================
HRESULT loadItem::init(string keyName, int width, int height)
{
	//�ε����� �ʱ�ȭ
	_kind = LOAD_KIND_IMAGE_0;
	//�̹��� ����ü �ʱ�ȭ
	_imageResource.keyName = keyName;
	_imageResource.width = width;
	_imageResource.height = height;

	return S_OK;
}

HRESULT loadItem::init(string keyName, const char * fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	//�ε����� �ʱ�ȭ
	_kind = LOAD_KIND_IMAGE_1;
	//�̹��� ����ü �ʱ�ȭ
	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.trans = isTrans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::init(string keyName, const char * fileName, float x, float y, int width, int height, bool isTrans, COLORREF transColor)
{
	//�ε����� �ʱ�ȭ
	_kind = LOAD_KIND_IMAGE_2;
	//�̹��� ����ü �ʱ�ȭ
	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.x = x;
	_imageResource.y = y;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.trans = isTrans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::init(string keyName, const char * fileName, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	//�ε����� �ʱ�ȭ
	_kind = LOAD_KIND_FRAMEIMAGE_0;
	//�̹��� ����ü �ʱ�ȭ
	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.frameX = frameX;
	_imageResource.frameY = frameY;
	_imageResource.trans = isTrans;
	_imageResource.transColor = transColor;

	return S_OK;
}

HRESULT loadItem::init(string keyName, const char * fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	//�ε����� �ʱ�ȭ
	_kind = LOAD_KIND_FRAMEIMAGE_1;
	//�̹��� ����ü �ʱ�ȭ
	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.x = x;
	_imageResource.y = y;
	_imageResource.width = width;
	_imageResource.height = height;
	_imageResource.frameX = frameX;
	_imageResource.frameY = frameY;
	_imageResource.trans = isTrans;
	_imageResource.transColor = transColor;

	return S_OK;
}


//=============================================================
//	## loading ## (�ε�Ŭ����)
//=============================================================
HRESULT loading::init(void)
{
	//�ε�ȭ�� ��׶��� �̹��� �ʱ�ȭ
	_background = IMAGEMANAGER->addImage("bgLoadingScene", "Texture/Loading/bgLoadingScene.bmp", WINSIZEX, WINSIZEY);

	//�ε��� Ŭ���� �ʱ�ȭ
	_loadingBar = new progressBar;
	_loadingBar->init("Texture/Loading/loadingBarFront", "Texture/Loading/loadingBarBack", 100, 430, 600, 20);
	_loadingBar->setGauge(0, 0);
	//���� ������ �ʱ�ȭ
	_currentGauge = 0;

	//�ε��� �ۼ�Ʈ �����̹��� �ʱ�ȭ
	_numbers = IMAGEMANAGER->addFrameImage("loadingNumber", "Texture/Loading/loadingNumber.bmp",400,60,10,1);
	
	//�ۼ�Ʈ ���� �ʱ�ȭ
	_percent = 0;

	return S_OK;
}

void loading::release(void)
{
	//�ε��� Ŭ���� ����
	
	_loadingBar->release();
	SAFE_DELETE(_loadingBar);
}

void loading::update(void)
{
	//�ε��� Ŭ���� ������Ʈ
	_loadingBar->update();

}

void loading::render(void)
{
	//��׶��� ����
	_background->render(getMemDC());
	//�ε��� Ŭ���� ����
	_loadingBar->render();

	//�ε� �ۼ�Ʈ �̹��� ����
	//�̹��� currentframeX�� �̿��ؼ� 0~9���� ǥ���ϸ� �ǰڴ�.
	//���� �ڸ�
	_numbers->frameRender(getMemDC(), _loadingBar->getRect().right - _numbers->getFrameWidth()*2, _loadingBar->getRect().top - _numbers->getFrameHeight(), (_percent % 100 / 10), 0);
	//���� �ڸ�
	_numbers->frameRender(getMemDC(), _loadingBar->getRect().right - _numbers->getFrameWidth(), _loadingBar->getRect().top - _numbers->getFrameHeight(), (_percent % 100 % 10), 0);

	char fileName[64];
	if (_currentGauge < _vLoadItem.size())
	{
		sprintf_s(fileName, "Image\\%s.bmp", _vLoadItem[_currentGauge]->getImageResource().keyName.c_str()); //c_str ��Ʈ���� const char*������
		TextOut(getMemDC(), _loadingBar->getRect().left + 10, _loadingBar->getRect().bottom + 20, fileName, strlen(fileName));
	}
}

void loading::loadImage(string keyName, int width, int height)
{
	loadItem* item = new loadItem;
	item->init(keyName, width, height);
	_vLoadItem.push_back(item);
}

void loading::loadImage(string keyName, const char * fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->init(keyName, fileName, width, height, isTrans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadImage(string keyName, const char * fileName, float x, float y, int width, int height, bool isTrans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->init(keyName, fileName, x, y, width, height, isTrans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadFrameImage(string keyName, const char * fileName, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->init(keyName, fileName, width, height, frameX, frameY, isTrans, transColor);
	_vLoadItem.push_back(item);
}

void loading::loadFrameImage(string keyName, const char * fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	loadItem* item = new loadItem;
	item->init(keyName, fileName, x, y, width, height, frameX, frameY, isTrans, transColor);
	_vLoadItem.push_back(item);
}

BOOL loading::loadingDone()
{
	//�ε��� �Ϸ��
	if (_currentGauge >= _vLoadItem.size())
	{
		return TRUE; //���� ��ȯ�ϱ� ���� true�� ������
	}

	loadItem* item = _vLoadItem[_currentGauge]; //�̹��� �ε��� ������������ ��������

	//���⿡�� loadItemŬ������ �����͸� ������ addimage, addsound�Ѵ�.
	switch (item->getLoadingKind())
	{
	case LOAD_KIND_IMAGE_0:
		{
			tagImageResource img = item->getImageResource();
			IMAGEMANAGER->addImage(img.keyName, img.width, img.height);
		}
		break;
	case LOAD_KIND_IMAGE_1:
		{
			tagImageResource img = item->getImageResource();
			IMAGEMANAGER->addImage(img.keyName, img.fileName, img.width, img.height, img.trans, img.transColor);
		}
		break;
	case LOAD_KIND_IMAGE_2:
		{	tagImageResource img = item->getImageResource();
			IMAGEMANAGER->addImage(img.keyName, img.fileName, img.x, img.y, img.width, img.height, img.trans, img.transColor);
		}	
		break;
	case LOAD_KIND_FRAMEIMAGE_0:
		{	tagImageResource img = item->getImageResource();
			IMAGEMANAGER->addFrameImage(img.keyName, img.fileName, img.width, img.height, img.frameX, img.frameY, img.trans, img.transColor);
		}	
		break;
	case LOAD_KIND_FRAMEIMAGE_1:
		{	tagImageResource img = item->getImageResource();
			IMAGEMANAGER->addFrameImage(img.keyName, img.fileName, img.x, img.y, img.width, img.height, img.frameX, img.frameY, img.trans, img.transColor);
		}	
		break;
	//�� ���� ���� ���� �˾Ƽ�...
	case LOAD_KIND_SOUND:
		break;
	}

	//switch������ �����͸� �ε��ϰ��ϸ� �ε��� ������ ����
	//��������� ����
	_currentGauge++;

	//�ε��ۼ�Ʈ
	_percent = ((float)_currentGauge / _vLoadItem.size() * 100); //int�� ������ / int�� ������ �����ϸ� �Ҽ����� ����������. -> �Ǽ������� ����ȯ �Ͽ� *100 ���� ������ �Ҽ����� �����ش�.

	//�ε��� �̹��� ����
	_loadingBar->setGauge(_currentGauge, _vLoadItem.size());

	return 0;
}


