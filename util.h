#pragma once

//=============================================================
//	## namespace MY_UTIL ##
//=============================================================

//=============================================================
//	## ���� ����ϴ� ��� ##
//=============================================================
#define MAGENTA		RGB(255,0,255)
#define PI		    3.14159265358979323846

#define PI_2		PI/2
#define PI_4		PI/4
#define PI_8		PI/8

namespace MY_UTIL
{
	// �� �� ������ �Ÿ�
	float getDistance(float startX, float startY, float endX, float endY);
	float getDistance(float _x, float _y);
	// �� �� ������ ����
	float getAngle(float startX, float startY, float endX, float endY);

	float getAnglef(float startX, float startY, float endX, float endY);
};

namespace utl = MY_UTIL;

