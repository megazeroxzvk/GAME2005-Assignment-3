#include "Jet.h"
#include "Config.h"
#include "Renderer.h"
#include "TextureManager.h"

Jet::Jet()
{
	TextureManager::Instance()->load(
		"../Assets/sprites/jet.png", "jet"
	);

	// set frame width
	setWidth(100);

	// set frame height
	setHeight(100);

	getTransform()->position = glm::vec2(400.0f, 500.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLANE);
}

Jet::~Jet()
= default;

void Jet::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the plane sprite with simple propeller animation
	TextureManager::Instance()->draw("jet", x, y, 0, 255, false, SDL_FLIP_NONE);
	const auto renderer = Renderer::Instance()->getRenderer();
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	if(debugView)
	{
		SDL_RenderDrawRect(renderer, &boundingBox1);
		SDL_RenderDrawRect(renderer, &boundingBox2);
	}
	
	//for(int i=0; i< 2; i++)
	//{
	//	SDL_RenderDrawRect(renderer, &boundingBox[i]);
	//}
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
}

void Jet::update()
{
	boundingBox1Update();
	boundingBox2Update();
	//boundingBoxUpdate();
}

void Jet::clean()
{
}

void Jet::jetMovementHandle(glm::vec2 mousePos)
{
	getTransform()->position.x = mousePos.x - (getWidth() * 0.5);
	if(getTransform()->position.x <= 0)
	{
		getTransform()->position.x = 0;
	}
	else if(getTransform()->position.x + getWidth() > Config::SCREEN_WIDTH)
	{
		getTransform()->position.x = Config::SCREEN_WIDTH - getWidth();
	}
	//std::cout << "Mouse x = " << getTransform()->position.x << std::endl;
}

void Jet::boundingBox1Update()
{
	boundingBox1.w = getWidth() * 0.25f;
	boundingBox1.h = getWidth() * 0.5f;
	boundingBox1.x = getTransform()->position.x + (getWidth() * 0.5f - boundingBox1.w * 0.5);
	boundingBox1.y = getTransform()->position.y;
}

void Jet::boundingBox2Update()
{
	boundingBox2.w = getWidth();
	boundingBox2.h = getWidth() * 0.5f;
	boundingBox2.x = getTransform()->position.x;
	boundingBox2.y = getTransform()->position.y + (getHeight()*0.5);
}

SDL_Rect Jet::getCollisionBox1()
{
	return boundingBox1;
}

SDL_Rect Jet::getCollisionBox2()
{
	return boundingBox2;
}
