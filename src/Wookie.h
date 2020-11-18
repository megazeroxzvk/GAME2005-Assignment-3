#pragma once
#ifndef __WOOKIE__
#define __WOOKIE__
#include "Sprite.h"


class Wookie : public Sprite
{
public:
	Wookie();
	~Wookie();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

private:
	const float m_scaleValue = 0.5;		//necessary ? 1p = 1m...
};

#endif /* defined (__WOOKIE__) */