#include "ThermalDetonator.h"
#include "TextureManager.h"
#include "Util.h"

ThermalDetonator::ThermalDetonator()
{
	TextureManager::Instance()->load(
		"../Assets/sprites/detonator.png", "detonator");

	getTransform()->position = glm::vec2(30.0f, 500.f);
	getRigidBody()->velocity = glm::vec2(0, 0);

	const auto size = TextureManager::Instance()->getTextureSize("detonator");
	getRigidBody()->isColliding = false;
	setWidth(size.x);
	setHeight(size.y);

	//Flat Surface y AXIS = 500 !
	getTransform()->position = glm::vec2(90.0f, 500.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->acceleration = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	//initial
	m_initialPosition = 90.0f;

	//reset
	m_resetPosition = glm::vec2(90.0f, 500.0f);
	m_resetAngle = 15.88963282f;
	m_resetSpeed = 95.0f;
	
	setType(PLAYER);

}

ThermalDetonator::~ThermalDetonator()
{

}

void ThermalDetonator::clean()
{

}


void ThermalDetonator::setDistance(float distance)
{
	this->m_distance = distance;
}

void ThermalDetonator::setAngle(float angle)
{
	this->m_angle = Util::Deg2Rad * angle;
}

void ThermalDetonator::setSpeed(float speed)
{
	this->m_speed = speed;

	//Setting Initial Velocity as soon as this function is run.
	//Vx = VcosOfTheta, Vy = VsinOfTheta
	getRigidBody()->velocity.x = speed * cos(m_angle);
	//SDL throwing upwards is actually is reducing y-axis value.
	getRigidBody()->velocity.y = -speed * sin(m_angle);
}


void ThermalDetonator::setTime(float angle)
{
	m_time = ((2.0 * m_speed * sin(Util::Deg2Rad * angle)) / gravity.y);
}


void ThermalDetonator::setPosition(glm::vec2 position)
{
	getTransform()->position = position;
}

float ThermalDetonator::getDistance()
{
	//pending - done
	m_distance = getTransform()->position.x - m_initialPosition;
	return m_distance;
}

float ThermalDetonator::getAngle()
{
	return Util::Rad2Deg * m_angle;
}

float ThermalDetonator::getSpeed()
{
	return m_speed;
}

glm::vec2 ThermalDetonator::getResetPositon()
{
	return m_resetPosition;
}

float ThermalDetonator::getResetAngle()
{
	return m_resetAngle;
}

float ThermalDetonator::getResetSpeed()
{
	return m_resetSpeed;
}

float ThermalDetonator::getTime()
{
	m_time = ((2.0 * m_speed * sin(m_angle))/gravity.y);
	m_landingLocation = (m_speed * cos(m_angle) * m_time) + m_initialPosition;
	return m_time;
}



float ThermalDetonator::getLandingLocation()
{
	return m_landingLocation;
}

void ThermalDetonator::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("detonator", x, y, 0, 255, true);
}

void ThermalDetonator::update()
{
	//physics time

	//An object that gets thrown off with a velocity, gravity and at a certain angle,
	//will come to the that same x-axis level and at the same speed due to
	//the acceleration and deceleration that the object gets.
	//Hence the detonator lands at the stormtroopers the landing velocity is equal to the starting velocity
	//And when it lands, booom!
	if(m_kickoff)
	{
		if (getRigidBody()->velocity.y > m_speed * sin(m_angle))
		{

			//time to stop.
			//getRigidBody()->velocity = { 0,0 };
			m_kickoff = false;
		}
		else
		{
			//std::cout << "speed.y = " << getRigidBody()->velocity.y << std::endl;
			// better way to do?? - yup, found the better way to do.
			//replaced 9.81 with gravity factor.
			getRigidBody()->velocity += (getRigidBody()->acceleration + gravity) * deltaTime;
			getTransform()->position += getRigidBody()->velocity * deltaTime;
		}
	}
	
}

