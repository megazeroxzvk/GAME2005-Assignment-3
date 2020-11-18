#pragma once
#ifndef __RAMP__
#define __RAMP__

#include "Sprite.h"

class Ramp : public Sprite
{
public:
	
	Ramp();
	~Ramp();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	//setters
	void setPositionBase1(glm::vec2 base1);
	void setPositionBase2(glm::vec2 base2);
	void setPositionTop1(glm::vec2 top1);

	//getters
	glm::vec2 getPositionBase1();
	glm::vec2 getPositionBase2();
	glm::vec2 getPositionTop1();
	float m_getAngle();
	float m_getBase();
	float m_getHeight();
	float m_getHypotenuse();
	void setNecessaryValues();
	void reset();
	
private:
	glm::vec2 positionBase1 = { 0,0 };
	glm::vec2 positionBase2 = { 0,0 };
	glm::vec2 positionTop1 = { 0,0 };
	glm::vec2 basePosition = { 0,0 };

	void drawRamp();

	//Physics-
	float m_angle;
	float m_base, m_height, m_hypotenuse;
	
};

#endif /* defined (__RAMP__) */