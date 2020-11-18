#include "Wookie.h"
#include "TextureManager.h"


Wookie::Wookie()
{
	TextureManager::Instance()->load("../Assets/sprites/wookie.png", "wookie");

	getTransform()->position = glm::vec2(30.0f, 500.f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	
	const auto size = TextureManager::Instance()->getTextureSize("wookie");
	getRigidBody()->isColliding = false;
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(50.0f, 520.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(TARGET);
}

Wookie::~Wookie()
{
	
}

void Wookie::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance()->draw("wookie", x, y, 0, 255, true);
}

void Wookie::update()
{
	
}


void Wookie::clean()
{
	
}


