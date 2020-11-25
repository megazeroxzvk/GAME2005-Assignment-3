#pragma once
#ifndef __BULLET__
#define __BULLET__

#include "Sprite.h"

class Bullet : public Sprite
{
public:
	Bullet();
	~Bullet();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void m_moveBullet();

	// reset
	void m_reset();

	// gravity adjustment
	void m_setGravity(float value);
	float m_getGravity();

	bool active = false;

	// bullet number
	int getBulletNumber();
	void setBulletNumber(int number);

	
private:
	int bulletNumber;
	float m_gravity = 9.8;
};

#endif /* defined (__BULLET__) */



