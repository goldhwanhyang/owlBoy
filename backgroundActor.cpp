#include "stdafx.h"
#include "backgroundActor.h"

void backgroundActor::render(int x, int y)
{
	_img->render(getMemDC(), x, y);
}
