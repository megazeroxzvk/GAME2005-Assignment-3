#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "Sprite.h"

class Target final : public Sprite {
public:
	Target();
	~Target();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	
	void m_move();

	// constant check bounds
	void m_checkBounds();
	void m_reset();


	// friction getters and setters
	float getFriction();
	void setFriction(float friction);

	// mass getters and setters
	float getMass();
	void setMass(float mass);
private:
	float m_friction;
	float m_mass;
};



#endif /* defined (__TARGET__) */