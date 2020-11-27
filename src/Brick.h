#pragma once
#ifndef __BRICK__
#define __BRICK__

#include "Sprite.h"
#include "Target.h"

class BoundingBox;

class Brick : public Sprite
{
public:
	Brick();
	~Brick();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	
	void BrickMovementHandle(glm::vec2);
	void CollisionCheckWithBoundingBoxes(Target*);
	void toggleRotate();

	// getters and setters for mass
	void setMass(float mass);
	float getMass();
	bool debugView;
	bool rotate = false;
	
private:
	BoundingBox* horizontalColliderBox;
	BoundingBox* verticalColliderBox;
	
	
protected:
	float m_velocityX, m_velocityY;
	float m_angle, speedFactor;
	void checkBounds();
	float m_mass;
};

class BoundingBox : public DisplayObject
{
public:
	
	BoundingBox(Brick*, bool);
	~BoundingBox();

	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	// getters and setters for mass
	void setMass(float mass);
	float getMass();
	
private:
	float m_mass;
	Brick* brickReference;
};
#endif /* defined (__BRICK__) */
