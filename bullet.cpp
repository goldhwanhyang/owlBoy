#include "stdafx.h"
#include "bullet.h"
HRESULT bullet::init(const char * imageName, int bulletMax, float range)
{
	_image = IMAGEMANAGER->findImage(imageName);
	_bulletMax = bulletMax;
	_range = range;

	return S_OK;
}

void bullet::update()
{

}

void bullet::render()
{

}

void bullet::release()
{

}

void bullet::fire()
{

}

void bullet::move()
{
}

void bullet::collide()
{
}
