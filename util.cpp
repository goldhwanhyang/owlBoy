#include "stdafx.h"
#include "util.h"

float MY_UTIL::getDistance(float startX, float startY, float endX, float endY)
{
	float x = endX - startX;
	float y = startY - endY;

	return sqrtf(x*x + y * y);
}

float MY_UTIL::getDistance(float _x, float _y)
{
	return sqrt(pow(_x, 2) + pow(_y, 2));
}

float MY_UTIL::getAngle(float startX, float startY, float endX, float endY)
{
	return atan2(startY - endY, endX - startX);
}

float MY_UTIL::getAnglef(float startX, float startY, float endX, float endY)
{
	float angle = atan2(startY - endY, endX - startX);
	if (angle < 0)
		angle += 2 * PI;
	return angle;
}
