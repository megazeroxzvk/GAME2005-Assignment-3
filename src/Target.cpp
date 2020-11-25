#include "Target.h"
#include "TextureManager.h"
#include "Util.h"


Target::Target()
{
	TextureManager::Instance()->load("../Assets/sprites/target.png","target");

	const auto size = TextureManager::Instance()->getTextureSize("target");
	setWidth(size.x);
	setHeight(size.y);
	std::cout << "Width of Target = " << getWidth() <<std::endl << "Height of Target = "<< getHeight() << std::endl;
	getTransform()->position = glm::vec2(100.0f, 100.0f);
	getRigidBody()->velocity = glm::vec2(0, 0); 
	getRigidBody()->acceleration = glm::vec2(10, 10);
	getRigidBody()->isColliding = false;
	m_friction = 0.5;
	setType(TARGET);
}

Target::~Target()
= default;

void Target::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("target", x, y, 0, 255, true);
	Util::DrawCircle(getTransform()->position, std::max(getWidth() * 0.5, getHeight() * 0.5), { 1,0,0,1 });
	m_mass = 5.0f;
}

void Target::update()
{
	// ------------- UPDATE THE TARGET VELOCITY AND ACCELERATION WITH A COEFFECIENT
	getRigidBody()->velocity += getRigidBody()->acceleration * deltaTime * m_friction;
	if(abs(getRigidBody()->velocity.x) <= 0.05)
	{
		getRigidBody()->velocity.x = 0;
		getRigidBody()->acceleration.x = 0;
	}
	if(abs(getRigidBody()->velocity.y) <= 0.05)
	{
		getRigidBody()->velocity.y = 0;
	}
	std::cout << "Velocity.X = " << getRigidBody()->velocity.x <<
		" Velocity.Y = " << getRigidBody()->velocity.y << std::endl;
	m_move();
	m_checkBounds();
	
}

void Target::clean()
{
}

void Target::m_move()
{
	getTransform()->position += getRigidBody()->velocity * deltaTime;
}

// keep the puck in the window
void Target::m_checkBounds()
{
	// base
	if (getTransform()->position.y + (getHeight() * 0.5) > 600.0f )
	{
		getRigidBody()->velocity = { getRigidBody()->velocity.x,-getRigidBody()->velocity.y };
	}

	// top
	if(getTransform()->position.y - (getHeight()*0.5) < 0)
	{
		getRigidBody()->velocity = { getRigidBody()->velocity.x,-getRigidBody()->velocity.y };
	}

	// right
	if(getTransform()->position.x + (getWidth()*0.5) > 800.0f)
	{
		getRigidBody()->velocity = { -getRigidBody()->velocity.x,getRigidBody()->velocity.y };
	}

	//left
	if(getTransform()->position.x - (getWidth()*0.5)< 0)
	{
		getRigidBody()->velocity = { -getRigidBody()->velocity.x,getRigidBody()->velocity.y };
	}
}

void Target::m_reset()
{

}

float Target::getFriction()
{
	return m_friction;
}

void Target::setFriction(float friction)
{
	m_friction = friction;
}
