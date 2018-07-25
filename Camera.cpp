#include "stdafx.h"
#include "Camera.h"

void Camera::videoShooting(int _x, int _y, float _speed)
{
	speed = _speed;
	if (speed != 0)
	{
		destX = _x - WINSIZEX / 2;
		destY = _y - WINSIZEY / 2;
		if (destX < 0)
		{
			destX = 0;
		}
		else if (destX + WINSIZEX > maxWidth)
		{
			destX = maxWidth - WINSIZEX;
		}
		if (destY < 0)
		{
			destY = 0;
		}
		else if (destY + WINSIZEY > maxHeight)
		{
			destY = maxHeight - WINSIZEY;
		}

	}
	else
	{
		x = destX = _x - WINSIZEX / 2;
		y = destY = _y - WINSIZEY / 2;
	}
	videoShooting();
}

void Camera::videoShooting()
{
	float angle;
	delay = 30;

	if(speed != 0)
		delayCount++;
	if (delayCount >= delay)
	{
		if ((x != destX || y != destY))
		{
			angle = getAngle(x, y, destX, destY);
			oldX = x;
			oldY = y;

			if(x != destX)
				x += cos(angle)*speed;
			if(y != destY)
				y += -sin(angle)*speed;

			if ((oldX < destX && destX < x) ||
				(oldX > destX && destX > x))
			{
				x = destX;
			}
			if ((oldY < destY && destY < y) ||
				(oldY > destY && destY > y))
			{
				y = destY;
			}
		}
		else
		{
			delayCount = 0;
		}
	}

	if (x < 0)
	{
		x = 0;
	}
	else if (x + WINSIZEX > maxWidth)
	{
		x = maxWidth - WINSIZEX;
	}
	if (y < 0)
	{
		y = 0;
	}
	else if (y + WINSIZEY > maxHeight)
	{
		y = maxHeight - WINSIZEY;
	}

	sX = sY = 0;
	if (count > 0)
	{
		count--;
		sX = RND->getInt(shakingPower) - shakingPower / 2;
		sY = RND->getInt(shakingPower) - shakingPower / 2;
	}

}

HRESULT Camera::init()
{
	x = 0;
	y = 0;
	maxWidth = WINSIZEX;
	maxHeight = WINSIZEY;
	count = 0;
	sX = sY = shakingPower = 0;
	delayCount = 0;
	return S_OK;
}

void Camera::release()
{
}
