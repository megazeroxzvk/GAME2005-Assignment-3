#pragma once
#ifndef __BRICK__
#define __BRICK__

#include "Sprite.h"
#include "Target.h"

class BoundingBox;

class Brick final : public Sprite
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
	
private:
	BoundingBox* horizontalColliderBox;
	BoundingBox* verticalColliderBox;
	bool rotate = false;
	float m_velocityX, m_velocityY;
	float m_angle, speedFactor;
	void checkBounds();
};

class BoundingBox : public DisplayObject
{
public:
	
	BoundingBox(Brick*, bool);
	~BoundingBox();

	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	
private:
	
};
#endif /* defined (__BRICK__) */
