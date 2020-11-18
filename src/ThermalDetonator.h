#pragma once
#ifndef __THERMAL_DETONATOR__
#define __THERMAL_DETONATOR__

#include "Sprite.h"


class ThermalDetonator : public Sprite
{
public:
	ThermalDetonator();
	~ThermalDetonator();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void setDistance(float distance);
	void setAngle(float angle);
	void setSpeed(float speed);
	void setPosition(glm::vec2 position);

	float getDistance();
	float getAngle();
	float getSpeed();

	//get 
	glm::vec2 getResetPositon();
	float getResetAngle();
	float getResetSpeed();

	//oh damn, forgot the one factor that cannot be taken back :)
	float getTime();
	void setTime(float angle);
	float getLandingLocation();
	bool m_kickoff = false;

private:
	//Physics stuff
	float m_distance, m_angle, m_speed;
	float m_initialPosition;
	float m_time;
	float m_landingLocation;

	//reset parameters
	glm::vec2 m_resetPosition = { 0,0 };
	float m_resetAngle;
	float m_resetSpeed;
};

#endif /* defined (__THERMAL_DETONATOR__) */
