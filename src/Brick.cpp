#include "Brick.h"

#include "CollisionManager.h"
#include "Config.h"
#include "Renderer.h"
#include "TextureManager.h"
#include "Util.h"

Brick::Brick()
{
	TextureManager::Instance()->load(
		"../Assets/sprites/brick.png", "brick"
	);

	const auto size = TextureManager::Instance()->getTextureSize("brick");
	setWidth(size.x);
	setHeight(size.y);
	std::cout << "Width of Brick = " << getWidth() << std::endl << "Height of Brick = " << getHeight() << std::endl;
	getTransform()->position = glm::vec2(400.0f, 500.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	m_mass = 10.0f;
	setType(BRICK);
	
	horizontalColliderBox = new BoundingBox(this, false);
	verticalColliderBox = new BoundingBox(this, true);
	
}

Brick::~Brick()
= default;

void Brick::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the plane sprite with simple propeller animation
	//Util::DrawRect(getTransform()->position, getWidth(), getHeight(), { 1,0,0,1 });
	//Util::DrawRect({ getTransform()->position.x + getWidth() * 0.5 - getHeight() * 0.5, getTransform()->position.y - getWidth() * 0.5 + getHeight() * 0.5}, getHeight(), getWidth(), { 0,1,0,1 });
	if (rotate)
	{
		TextureManager::Instance()->draw("brick", x, y, 90, 255, false, SDL_FLIP_NONE);
	}
	else
	{
		TextureManager::Instance()->draw("brick", x, y, 0, 255, false, SDL_FLIP_NONE);
	}

	const auto renderer = Renderer::Instance()->getRenderer();

	if (debugView)
	{
		horizontalColliderBox->draw();
		verticalColliderBox->draw();
	}
		
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);	
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

}

void Brick::update()
{
	// Movement Update
	verticalColliderBox->update();
	horizontalColliderBox->update();
	verticalColliderBox->getTransform()->position = { this->getTransform()->position.x + this->getWidth() * 0.5 - this->getHeight() * 0.5,
		this->getTransform()->position.y - this->getWidth() * 0.5 + this->getHeight() * 0.5 };
	horizontalColliderBox->getTransform()->position = getTransform()->position;
	
}

void Brick::clean()
{
}

void Brick::BrickMovementHandle(glm::vec2 mousePos)
{
	/*std::cout << "Brick Position X = " << getTransform()->position.x
		<< "Brick Position Y = " << getTransform()->position.y << std::endl;

	std::cout << "Mouse Position X = " << mousePos.x
		<< "Mouse Position Y = " << mousePos.y << std::endl;*/

	
	
	glm::vec2 distance = {mousePos.x - (getTransform()->position.x + getWidth()*0.5),
		mousePos.y - (getTransform()->position.y+getHeight()*0.5)};


	// Magnitude to take the speed factor and convert it into reasonable scale 
	speedFactor = Util::magnitude(distance) * 0.1f;
	if(speedFactor < 0.1)
	{
		speedFactor = 0;
	}
	//std::cout << "distance.x = " << distance.x  << "distance.y = " << distance.y << std::endl;
	float angle = atan2(distance.y, distance.x) * Util::Rad2Deg;
	/*std::cout << "Angle = " <<   
	angle
		<< std::endl;*/

	glm::vec2 direction;
	direction.x = cos(angle * Util::Deg2Rad);
	direction.y = sin(angle * Util::Deg2Rad);

	/*std::cout << "Direction Vector X = " <<
		direction.x << " Direction Vector Y = " << direction.y << std::endl;*/

	
	//std::cout << "Speed Factor = " << speedFactor << std::endl;
	// Stuttering Issue when using direction vector.
	getRigidBody()->velocity = { speedFactor * direction.x,speedFactor * direction.y};
	//std::cout << "Brick VelX = " << getRigidBody()->velocity.x << "Brick VelY = " << getRigidBody()->velocity.y << std::endl;
	getTransform()->position += getRigidBody()->velocity;

	
	//std::cout << Util::magnitude(getRigidBody()->velocity) << std::endl;
	checkBounds();
	
	
}

void Brick::CollisionCheckWithBoundingBoxes(Target*  target)
{
	if(rotate)
	{
		CollisionManager::circleAABBCheck(target, verticalColliderBox, rotate);
	}
	else
	{
		CollisionManager::circleAABBCheck(target, horizontalColliderBox, rotate);
	}
	
}

void Brick::toggleRotate()
{
	rotate = !rotate;
}

void Brick::setMass(float mass)
{
	m_mass = mass;
}

float Brick::getMass()
{
	return m_mass;
}

void Brick::checkBounds()
{
	if (!rotate)
	{
		//	getTransform()->position.x = mousePos.x - (getWidth() * 0.5f);
		if (getTransform()->position.x <= 0)
		{
			getTransform()->position.x = 0;
		}
		if (getTransform()->position.x + getWidth() > Config::SCREEN_WIDTH)
		{
			getTransform()->position.x = Config::SCREEN_WIDTH - getWidth();
		}

		//	getTransform()->position.y = mousePos.y - (getHeight() * 0.5f);
		if (getTransform()->position.y <= 0)
		{
			getTransform()->position.y = 0;
		}
		if (getTransform()->position.y + getHeight() > Config::SCREEN_HEIGHT)
		{
			getTransform()->position.y = Config::SCREEN_HEIGHT - getHeight();
		}
		//std::cout << "Mouse x = " << getTransform()->position.x << std::endl;
	}
	else
	{
		//	getTransform()->position.x = mousePos.x - (getWidth() * 0.5);
		if (getTransform()->position.x <= -getWidth() * 0.5f + getHeight() * 0.5f)
		{
			getTransform()->position.x = -getWidth() * 0.5f + getHeight() * 0.5f;
		}
		if (getTransform()->position.x > Config::SCREEN_WIDTH - getWidth() * 0.5f - getHeight() * 0.5f)
		{
			getTransform()->position.x = Config::SCREEN_WIDTH - getWidth() * 0.5 - getHeight() * 0.5f;
		}

		//	getTransform()->position.y = mousePos.y - (getHeight() * 0.5f);
		if (getTransform()->position.y <= 0 + getWidth() * 0.5f - getHeight() * 0.5f)
		{
			getTransform()->position.y = 0 + getWidth() * 0.5f - getHeight() * 0.5f;
		}
		if (getTransform()->position.y > Config::SCREEN_HEIGHT - getWidth() * 0.5f - getHeight() * 0.5)
		{
			getTransform()->position.y = Config::SCREEN_HEIGHT - getWidth() * 0.5f - getHeight() * 0.5;
		}
	}

}

BoundingBox::BoundingBox(Brick* brickRef, bool rotate)
{
	setType(BRICK);
	brickReference = brickRef;
	if(!rotate)
	{
		getTransform()->position = brickReference->getTransform()->position;
		setWidth(brickReference->getWidth());
		setHeight(brickReference->getHeight());
	}
	else
	{
		getTransform()->position =
		{
			brickReference->getTransform()->position.x + brickReference->getWidth() * 0.5 - brickReference->getHeight() * 0.5,
			brickReference->getTransform()->position.y - brickReference->getWidth() * 0.5 + brickReference->getHeight() * 0.5
		};
		setWidth(brickReference->getHeight());
		setHeight(brickReference->getWidth());
	}
	
	
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(BRICK);
	m_mass = brickReference->getMass();
	
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	Util::DrawRect(getTransform()->position, getWidth(), getHeight(), { 1,0,1,1 });
}

void BoundingBox::update()
{
	getRigidBody()->velocity = brickReference->getRigidBody()->velocity;
}

void BoundingBox::clean()
{
}

void BoundingBox::setMass(float mass)
{
	m_mass = mass;
}

float BoundingBox::getMass()
{
	return m_mass;
}

