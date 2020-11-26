#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "CollisionShape.h"
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

	// Shape change Collision Type set... // complete circle first, then shift to box
	CollisionShape getCollisionShape();
	void setCollisionShape(CollisionShape CircleOrSquare);

	// friction getters and setters
	float getFriction();
	void setFriction(float friction);

	// mass getters and setters
	float getMass();
	void setMass(float mass);
private:
	float m_friction;
	float m_mass;
	CollisionShape m_shape;
};



#endif /* defined (__TARGET__) */