// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// WindowAPI.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"

// TODO: 필요한 추가 헤더는
// 이 파일이 아닌 STDAFX.H에서 참조합니다.


//ADDED
/*
SetBkMode TextOut뒷배경 투명
SetBkMode OPAQUE -불투명
SetBkMode TRANSPARENT 투명배경
wsprintf(char배열변수, "%d", "TEXT");
TextOut(hdc,x, y, "TEXT", strlen("TEXT"));

wsprintf(_debug, "%d", 변수);
TextOut(memDC,x, y, _debug, strlen(_debug));
_stprintf_s(_debug, "%f", 변수);
TextOut(memDC,x, y, _debug, strlen(_debug));

PtInRect (마우스,렉트) 충돌

IntersectRect(충돌한영역넓이주소,rc1주소,rc2주소);

SetDCBrushColor(hdc, RGB(r, g, b));
SelectObject(hdc, GetStockObject(DC_BRUSH));

memset(구조체주소,초기화값(0),sizeof(struct tag~구조체이름));
memset(배열도 가능);
Zeromemory(,);

클래스 추가하려면 gameNode클래스를 상속받아서 만들면 된다.
mainGame클래스를 참고해서 init{return S_OK},release,update,render를 만들고 mainGame클래스 안에 동적할당,SAFE_DELETE,업데이트,렌더를 만들면 된다.
*/