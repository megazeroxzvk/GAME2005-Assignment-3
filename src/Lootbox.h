#pragma once
#ifndef __LOOTBOX__
#define __LOOTBOX__

#include "Sprite.h"

class Lootbox : public Sprite
{
public:
	Lootbox();
	~Lootbox();
	
	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	glm::vec2 getPosition();
	float getAngle();
	
	void setPosition(float x, float y);
	void setAngle(float angle);
	void setAcceleration(glm::vec2 acc);
	
	float getDistanceTravelledRamp(glm::vec2, glm::vec2);
	float getDistanceTravelledFrictionSurface(glm::vec2, glm::vec2);

	void setNecessaryValues(float angle,float hypotenuse);

	void setMass(float mass);
	float getMass();
	bool startSimulation = false;
	void reset(float,float);
	void resetPosition(float x, float y);

	void setCoefficientOfFriction(float val);
	float getCoefficientOfFriction();

	void setReachedLocation(bool);
	bool getReachedLocation();

	glm::vec2 getForce();

	float velocityLabel = 0.0f;
	float getDistance();
	
private:
	//physics !!!!!!!!!!!!
	float m_angle;
	SDL_Point bottomLeft = { 0, 0 };
	SDL_Point bottomRight = { 0,0 };
	SDL_Rect destRect;
	float m_rampHypotenuse = 0.0f;;

	//for now we keep this constant
	float m_mass = 12.8f;
	glm::vec2 m_force = {0,0};
	float m_coefficient = 0.42f;
	
	glm::vec2 m_initialPosition = { 0,0 };
	float m_distanceTravelledRamp = 0;
	float m_distanceTravelledFrictionSurface = 0;
	bool reachedLocation = false;

	bool m_reachedRampBase = false;
	glm::vec2 m_rampBasePosition = {0,0};
	glm::vec2 m_frictionSurfaceEndPosition = { 0,0 };
	
};

#endif /* defined (__LOOTBOX__) */
