#include "Target.h"

#include "SoundManager.h"
#include "TextureManager.h"
#include "Util.h"


Target::Target()
{
	TextureManager::Instance()->load("../Assets/sprites/targetCircle.png","targetCircle");
	TextureManager::Instance()->load("../Assets/sprites/targetSquare.png","targetSquare");

	SoundManager::Instance().load("../Assets/audio/ballwall.wav", "ballwall", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/goal.wav", "goal", SOUND_SFX);

	// Set to circle as default
	const auto size = TextureManager::Instance()->getTextureSize("targetCircle");
	setWidth(size.x);
	setHeight(size.y);
	std::cout << "Width of Target = " << getWidth() <<std::endl << "Height of Target = "<< getHeight() << std::endl;
	getTransform()->position = glm::vec2(100.0f, 100.0f);
	getRigidBody()->velocity = glm::vec2(10, 10); 
	getRigidBody()->acceleration = glm::vec2(10, 10);
	getRigidBody()->isColliding = false;
	m_friction = 0.8f;
	m_mass = 5.0f;
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
	switch (m_shape)
	{
	case CIRCLE:
		TextureManager::Instance()->draw("targetCircle", x, y, 0, 255, true);
		if(debugView)
		{
			Util::DrawCircle(getTransform()->position, std::max(getWidth() * 0.5f, getHeight() * 0.5f), { 1,0,0,1 });
		}
		break;

	case RECTANGLE:
		TextureManager::Instance()->draw("targetSquare", x, y, 0, 255, true);
		if(debugView)
		{
			Util::DrawRect({ getTransform()->position.x - getWidth() * 0.5f, getTransform()->position.y - getHeight() * 0.5f },
				getWidth(), getHeight(), { 1,0,0,1 });
		}
		break;

	default:
		TextureManager::Instance()->draw("targetCircle", x, y, 0, 255, true);
		if(debugView)
		{
			Util::DrawCircle(getTransform()->position, std::max(getWidth() * 0.5f, getHeight() * 0.5f),
				{ 1,0,0,1 });
		}
		break;
	}
	
	
}

void Target::update()
{
	// ------------- UPDATE THE TARGET VELOCITY AND ACCELERATION WITH A COEFFECIENT OF FRICTION

	// Try different variations
	//getRigidBody()->velocity += getRigidBody()->acceleration * deltaTime;
	//if(abs(getRigidBody()->velocity.x) <= 0.05)
	//{
	//	getRigidBody()->velocity.x = 0;
	//	getRigidBody()->acceleration.x = 0;
	//}
	//if(abs(getRigidBody()->velocity.y) <= 0.05)
	//{
	//	getRigidBody()->velocity.y = 0;
	//}
	//std::cout << "Velocity.X = " << getRigidBody()->velocity.x <<
	//	" Velocity.Y = " << getRigidBody()->velocity.y << std::endl;
	m_move();
	m_checkBounds();
	
}

void Target::clean()
{
}

void Target::m_move()
{
	getTransform()->position += getRigidBody()->velocity;
	//std::cout << Util::magnitude(getRigidBody()->velocity) << std::endl;
	if(Util::magnitude(getRigidBody()->velocity) < 0.5f)
	{
		getRigidBody()->velocity = { 0,0 };
	}
}

// keep the puck in the window
void Target::m_checkBounds()
{
	// base
	if (getTransform()->position.y + (getHeight() * 0.5) >= 600.0f )
	{
		getTransform()->position.y = 600.0f - (getHeight() * 0.5f);
		getRigidBody()->velocity = { getRigidBody()->velocity.x,-getRigidBody()->velocity.y };
		getRigidBody()->velocity *= m_friction;
		SoundManager::Instance().playSound("ballwall", 0, 1);
	}

	// top
	if(getTransform()->position.y - (getHeight() * 0.5f) <= 0.0f)
	{
		getTransform()->position.y = getHeight() * 0.5f;
		getRigidBody()->velocity = { getRigidBody()->velocity.x,-getRigidBody()->velocity.y };
		getRigidBody()->velocity *= m_friction;
		SoundManager::Instance().playSound("ballwall", 0, 1);
	}

	// right
	if(getTransform()->position.x + (getWidth() * 0.5f) >= 800.0f)
	{
		getTransform()->position.x = 800.0f - (getWidth() * 0.5f);
		getRigidBody()->velocity = { -getRigidBody()->velocity.x,getRigidBody()->velocity.y };
		getRigidBody()->velocity *= m_friction;
		if (getTransform()->position.y >= 240 && getTransform()->position.y <= (380))
		{
			pointsScored <= 0 ? 0 : pointsScored--;
			SoundManager::Instance().playSound("goal", 0, 1);
		}
		else
		{
			SoundManager::Instance().playSound("ballwall", 0, 1);
		}
	}
	
	//left
	if(getTransform()->position.x - (getWidth() * 0.5f) <= 0.0f)
	{
		getTransform()->position.x = (getWidth() * 0.5f);
		getRigidBody()->velocity = { -getRigidBody()->velocity.x,getRigidBody()->velocity.y };
		getRigidBody()->velocity *= m_friction;

		if(getTransform()->position.y >= 240 && getTransform()->position.y <= (380))
		{
			pointsScored++;
			SoundManager::Instance().playSound("goal", 0, 1);
		}
		else
		{
			SoundManager::Instance().playSound("ballwall", 0, 1);
		}
	}
	
	
	//std::cout << "Points Scored = " << pointsScored << std::endl;
	
	//std::cout << "Velocity X = " << getRigidBody()->velocity.x << "\nVelocity Y = " << getRigidBody()->velocity.y << std::endl;
}

void Target::m_reset()
{

}

CollisionShape Target::getCollisionShape()
{
	return m_shape;
}

void Target::setCollisionShape(CollisionShape CircleOrSquare)
{
	m_shape = CircleOrSquare;
}

float Target::getFriction()
{
	return m_friction;
}

void Target::setFriction(float friction)
{
	m_friction = friction;
}

float Target::getMass()
{
	return m_mass;
}

void Target::setMass(float mass)
{
	m_mass = mass;
}
