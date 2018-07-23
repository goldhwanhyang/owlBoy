#include "stdafx.h"
#include "loading.h"
//=============================================================
//	## loadItem ## (로드아이템 클래스)
//=============================================================
HRESULT loadItem::init(string keyName, int width, int height)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_IMAGE_0;
	//이미지 구조체 초기화
	_imageResource.keyName = keyName;
	_imageResource.width = width;
	_imageResource.height = height;

	return S_OK;
}

HRESULT loadItem::init(string keyName, const char * fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_IMAGE_1;
	//이미지 구조체 초기화
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
	//로딩종류 초기화
	_kind = LOAD_KIND_IMAGE_2;
	//이미지 구조체 초기화
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
	//로딩종류 초기화
	_kind = LOAD_KIND_FRAMEIMAGE_0;
	//이미지 구조체 초기화
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
	//로딩종류 초기화
	_kind = LOAD_KIND_FRAMEIMAGE_1;
	//이미지 구조체 초기화
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
//	## loading ## (로딩클래스)
//=============================================================
HRESULT loading::init(void)
{
	//로딩화면 백그라운드 이미지 초기화
	_background = IMAGEMANAGER->addImage("bgLoadingScene", "Texture/Loading/bgLoadingScene.bmp", WINSIZEX, WINSIZEY);

	//로딩바 클래스 초기화
	_loadingBar = new progressBar;
	_loadingBar->init("Texture/Loading/loadingBarFront", "Texture/Loading/loadingBarBack", 100, 430, 600, 20);
	_loadingBar->setGauge(0, 0);
	//현재 게이지 초기화
	_currentGauge = 0;

	//로딩바 퍼센트 숫자이미지 초기화
	_numbers = IMAGEMANAGER->addFrameImage("loadingNumber", "Texture/Loading/loadingNumber.bmp",400,60,10,1);
	
	//퍼센트 숫자 초기화
	_percent = 0;

	return S_OK;
}

void loading::release(void)
{
	//로딩바 클래스 해제
	
	_loadingBar->release();
	SAFE_DELETE(_loadingBar);
}

void loading::update(void)
{
	//로딩바 클래스 업데이트
	_loadingBar->update();

}

void loading::render(void)
{
	//백그라운드 렌더
	_background->render(getMemDC());
	//로딩바 클래스 렌더
	_loadingBar->render();

	//로딩 퍼센트 이미지 렌더
	//이미지 currentframeX를 이용해서 0~9까지 표현하면 되겠다.
	//십의 자리
	_numbers->frameRender(getMemDC(), _loadingBar->getRect().right - _numbers->getFrameWidth()*2, _loadingBar->getRect().top - _numbers->getFrameHeight(), (_percent % 100 / 10), 0);
	//일의 자리
	_numbers->frameRender(getMemDC(), _loadingBar->getRect().right - _numbers->getFrameWidth(), _loadingBar->getRect().top - _numbers->getFrameHeight(), (_percent % 100 % 10), 0);

	char fileName[64];
	if (_currentGauge < _vLoadItem.size())
	{
		sprintf_s(fileName, "Image\\%s.bmp", _vLoadItem[_currentGauge]->getImageResource().keyName.c_str()); //c_str 스트링을 const char*형으로
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
	//로딩이 완료됨
	if (_currentGauge >= _vLoadItem.size())
	{
		return TRUE; //씬을 전환하기 위해 true를 리턴함
	}

	loadItem* item = _vLoadItem[_currentGauge]; //이번에 로딩할 데이터정보를 가져오자

	//여기에서 loadItem클래스의 데이터를 가지고 addimage, addsound한다.
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
	//곧 사운드 배우고 난후 알아서...
	case LOAD_KIND_SOUND:
		break;
	}

	//switch문으로 데이터를 로드하고하면 로딩바 게이지 증가
	//현재게이지 증가
	_currentGauge++;

	//로딩퍼센트
	_percent = ((float)_currentGauge / _vLoadItem.size() * 100); //int형 게이지 / int형 사이즈 연산하면 소숫점을 버려버린다. -> 실수형으로 형변환 하여 *100 연산 이전의 소수점을 남겨준다.

	//로딩바 이미지 변경
	_loadingBar->setGauge(_currentGauge, _vLoadItem.size());

	return 0;
}


