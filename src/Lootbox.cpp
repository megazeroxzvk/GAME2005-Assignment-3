#include "Lootbox.h"

#include "Renderer.h"
#include "TextureManager.h"
#include "Util.h"

Lootbox::Lootbox()
{
	TextureManager::Instance()->load(
		"../Assets/sprites/owlootbox.png", "lootbox");

	//const auto size = TextureManager::Instance()->getTextureSize("lootbox");

	setWidth(50);
	setHeight(50);

	m_angle = 0;
	setType(PLAYER);
	
}

Lootbox::~Lootbox()
{
	
}

void Lootbox::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	SDL_Rect srcRect;
	
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = 180;
	srcRect.h = 180;

	destRect.x = x;
	destRect.y = y;
	destRect.w = getWidth();
	destRect.h = getHeight();

	//std::cout << "Position of lootBox = " <<x << ", " << y << std::endl;
	
	// draw
	//TextureManager::Instance()->draw("lootbox", x, y, m_angle, 255, false,SDL_FLIP_NONE);
	SDL_RenderCopyEx(Renderer::Instance()->getRenderer(), TextureManager::Instance()->getTexture("lootbox"), &srcRect, &destRect, m_angle, &bottomLeft, SDL_FLIP_NONE);
}

void Lootbox::update()
{
	bottomLeft = { 0, destRect.h };
	bottomRight = { destRect.w, destRect.h };

	//simulation start
	if(startSimulation && !reachedLocation)
	{
		//Friction Surface	__
		if ( getTransform()->position.y  > 499.0f)
		{
			setAngle(0.00f);
			if(!m_reachedRampBase)
			{
				getRigidBody()->velocity.x = sqrt(2 * getRigidBody()->acceleration.y * m_rampHypotenuse);
				m_distanceTravelledRamp = getDistanceTravelledRamp(m_initialPosition, getTransform()->position);
				m_rampBasePosition = getTransform()->position;
				m_reachedRampBase = true;
			}
			//friction 
			getRigidBody()->velocity.x += (-gravity.y * SCALE) * deltaTime * m_coefficient;
			getTransform()->position.x += getRigidBody()->velocity.x * deltaTime;
			
			
			if(getRigidBody()->velocity.x <= 0.0f)
			{
				getRigidBody()->velocity.x = 0.0f;
				m_distanceTravelledFrictionSurface = getDistanceTravelledFrictionSurface(m_rampBasePosition, getTransform()->position);
				reachedLocation = true;
			}
			m_distanceTravelledFrictionSurface = getDistanceTravelledFrictionSurface(m_rampBasePosition, getTransform()->position);
			
		}
		else		//Frictionless Ramp 
		{
			getRigidBody()->velocity += (getRigidBody()->acceleration) * deltaTime;
			getTransform()->position += getRigidBody()->velocity * deltaTime;
			if(getTransform()->position.y > 550.0f - getHeight()*1.88f )
			{
				m_angle--;
			}
			m_distanceTravelledRamp = getDistanceTravelledRamp(m_initialPosition, getTransform()->position);
		}
	}
}

void Lootbox::clean()
{
}

glm::vec2 Lootbox::getPosition()
{
	return getTransform()->position;
}

void Lootbox::setPosition(float x, float y)
{
	getTransform()->position.x = m_initialPosition.x = x;
	getTransform()->position.y = m_initialPosition.y = y;
}


float Lootbox::getAngle()
{
	return m_angle;
}

void Lootbox::setAngle(float angle)
{
	m_angle = angle;
}


void Lootbox::setAcceleration(glm::vec2 acc)
{
	getRigidBody()->acceleration.x = acc.x;
	getRigidBody()->acceleration.y = acc.y;
//	std::cout << "Acc = " << getRigidBody()->acceleration.y << std::endl;
}

float Lootbox::getDistanceTravelledRamp(glm::vec2 position1, glm::vec2 position2)
{
	float distance = sqrt((
		(position2.x - position1.x) * (position2.x - position1.x)) +
		(((position2.y + getHeight()) - (position1.y + getHeight())) * ((position2.y + getHeight()) - (position1.y + getHeight())))) / SCALE;
	
	//std::cout << "distance = " << distance << std::endl;
	return distance;
}

float Lootbox::getDistanceTravelledFrictionSurface(glm::vec2 position1, glm::vec2 position2)
{
	float distance = (position2.x - position1.x) / SCALE;

	//std::cout << "distance = " << distance << std::endl;
	return distance;
}

void Lootbox::setNecessaryValues(float angle,float hypotenuse)
{
	setAngle(angle);
	m_rampHypotenuse = hypotenuse;
	m_force.x = m_mass * gravity.y * (sin(Util::Deg2Rad * m_angle)) * SCALE;
	m_force.y = m_mass * gravity.y * (cos(Util::Deg2Rad * m_angle)) * SCALE;
	//********************* magical numbers ********************/
	setAcceleration({ (m_force.y / m_mass), (m_force.x / m_mass)});

	//setting total time
	float vel1 = 0.0f;
	float time1 = 0.0f;
	float acc1 = 0.0f;
	vel1 = sqrt(2 * getRigidBody()->acceleration.y * m_rampHypotenuse);
	vel1 /= SCALE;
	velocityLabel = vel1;
}

void Lootbox::reset(float x, float y)
{
	setMass(12.8f);
	setPosition(x, y);
	setCoefficientOfFriction(0.42f);
	m_angle = 0.0f;
	getRigidBody()->velocity = { 0,0 };
	getRigidBody()->acceleration = { 0,0 };
	reachedLocation = false;
	m_reachedRampBase = false;
	m_distanceTravelledRamp = 0.0f;
	m_distanceTravelledFrictionSurface = 0.0f;
	m_force = { 0.0,0.0 };
}

void Lootbox::resetPosition(float x, float y)
{
	setPosition(x, y);
	m_angle = 0.0f;
	getRigidBody()->velocity = { 0,0 };
	getRigidBody()->acceleration = { 0,0 };
	reachedLocation = false;
	m_reachedRampBase = false;
	m_distanceTravelledRamp = 0.0f;
	m_distanceTravelledFrictionSurface = 0.0f;
	m_force = { 0.0,0.0 };
}


glm::vec2 Lootbox::getForce()
{
	return m_force;
}

float Lootbox::getDistance()
{
	return m_distanceTravelledFrictionSurface + m_distanceTravelledRamp;
}

void Lootbox::setMass(float mass)
{
	m_mass = mass;
}

float Lootbox::getMass()
{
	return m_mass;
}


void Lootbox::setCoefficientOfFriction(float val)
{
	m_coefficient = val;
}

float Lootbox::getCoefficientOfFriction()
{
	return m_coefficient;
}

void Lootbox::setReachedLocation(bool reachedLocation)
{
	this->reachedLocation = reachedLocation;
}

bool Lootbox::getReachedLocation()
{
	return reachedLocation;
}


