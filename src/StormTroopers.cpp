#include "StormTroopers.h"
#include "TextureManager.h"
#include "Util.h"

StormTroopers::StormTroopers()
{
	TextureManager::Instance()->load(
		"../Assets/sprites/storm_troopers.png", "storm_troopers");

	getTransform()->position = glm::vec2(30.0f, 500.f);
	getRigidBody()->velocity = glm::vec2(0, 0);

	const auto size = TextureManager::Instance()->getTextureSize("storm_troopers");
	getRigidBody()->isColliding = false;
	setWidth(size.x);
	setHeight(size.y);

	//Flat Surface y AXIS = 500 !
	getTransform()->position = glm::vec2(575, 500.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	m_resetPosition = 575;
	
	setType(TARGET);
}

StormTroopers::~StormTroopers()
{
	
}


void StormTroopers::update()
{
	
}

void StormTroopers::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("storm_troopers", x, y, 0, 255, true);
}


void StormTroopers::clean()
{
	
}

float StormTroopers::getResetPosition()
{
	return m_resetPosition;
}

