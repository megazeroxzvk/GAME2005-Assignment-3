#include "Background.h"

#include "TextureManager.h"

//constructor
Background::Background(std::string m_path, std::string m_name)
{
	TextureManager::Instance()->load(m_path, m_name);
		
	setWidth(Config::SCREEN_WIDTH);
	setHeight(Config::SCREEN_HEIGHT);
	this->m_name = m_name;
	getTransform()->position = { 0,0 };
	getRigidBody()->isColliding = false;
}

//clean
void Background::clean()
{
	
}

//draw
void Background::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the background
	TextureManager::Instance()->draw(m_name, x, y, 0, 255, false);
}

//update
void Background::update()
{
	
}

//destructor
Background::~Background()
{
	
}