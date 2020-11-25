#include "Bullet.h"
#include "TextureManager.h"
#include "Util.h"

Bullet::Bullet()
{
	TextureManager::Instance()->load(
		"../Assets/textures/laser.png", "laser"
	);
	// set frame width
	setWidth(13);

	// set frame height
	setHeight(14);

	getTransform()->position = glm::vec2(200.0f, 200.0f);
	setType(BULLET);
	// reset to set values.
	m_reset();
}

Bullet::~Bullet()
= default;

void Bullet::draw()
{
	if(active)
	{
		// alias for x and y
		const auto x = getTransform()->position.x;
		const auto y = getTransform()->position.y;

		// draw the plane sprite with simple propeller animation
		TextureManager::Instance()->draw("laser", x, y, 0, 255, false, SDL_FLIP_NONE);
	}
}

void Bullet::update()
{
	if(getTransform()->position.y > Config::SCREEN_HEIGHT)
	{
		active = false;
	}
	
	if(active)
	{
		m_moveBullet();
	}
	
}

void Bullet::clean()
{
	
}

// reset to default 9.8 m/s^2, active and velocity
void Bullet::m_reset()
{
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	//gravity
	m_gravity = 9.8;
	getRigidBody()->acceleration = glm::vec2(0.0f, m_gravity);
	getTransform()->position = glm::vec2(Util::RandomRange(0, Config::SCREEN_WIDTH - 15), 0.0f);
	getRigidBody()->isColliding = false;
	active = false;
}

// move bullet as per gravity
void Bullet::m_moveBullet()
{
	getRigidBody()->velocity = getRigidBody()->velocity + (getRigidBody()->acceleration * deltaTime);
	getTransform()->position = getTransform()->position + (getRigidBody()->velocity * deltaTime);
}



// adjust gravity
void Bullet::m_setGravity(float value)
{
	m_gravity = value;
}

float Bullet::m_getGravity()
{
	return m_gravity;
}

int Bullet::getBulletNumber()
{
	return bulletNumber;
}

void Bullet::setBulletNumber(int number)
{
	bulletNumber = number;
}

