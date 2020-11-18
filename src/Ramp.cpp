#include "Ramp.h"

#include <iostream>

#include "Util.h"

Ramp::Ramp()
{

	getTransform()->position = glm::vec2(100.0f, 100.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);


	//Util::DrawLine({ 100,100 }, { 500,100 }, { 0,1,0,1 });
	positionBase1 = { 50,550 };
	positionBase2 = { 170,550 };
	positionTop1 = { 50,460 };
	basePosition = { 0, 550 };
}

Ramp::~Ramp()
{
}

void Ramp::draw()
{
	drawRamp();
}

void Ramp::update()
{
	//setNecessaryValues();
	m_base = positionBase2.x - positionBase1.x;
	m_height = positionBase1.y - positionTop1.y;

	m_hypotenuse = sqrt((m_base * m_base) + (m_height * m_height));
	m_angle = asin(m_height / m_hypotenuse);
	//std::cout << "base = " << m_base << "\theight = " << m_height << "\thypo = " << m_hypotenuse << 
		//"\tangle = " << Util::Rad2Deg * m_angle << std::endl;
	
}

void Ramp::clean()
{
}

void Ramp::setPositionBase1(glm::vec2 base1)
{
	positionBase1 = base1;
}

void Ramp::setPositionBase2(glm::vec2 base2)
{
	positionBase2 = base2;
}

void Ramp::setPositionTop1(glm::vec2 top1)
{
	positionTop1 = top1;
}

glm::vec2 Ramp::getPositionBase1()
{
	return positionBase1;
}

glm::vec2 Ramp::getPositionBase2()
{
	return positionBase2;
}

glm::vec2 Ramp::getPositionTop1()
{
	return positionTop1;
}

float Ramp::m_getAngle()
{
	return (Util::Rad2Deg * m_angle);
}

float Ramp::m_getBase()
{
	return m_base;
}

float Ramp::m_getHeight()
{
	return m_height;
}

float Ramp::m_getHypotenuse()
{
	return m_hypotenuse;
}

void Ramp::setNecessaryValues()
{
	m_base = positionBase2.x - positionBase1.x;
	//std::cout << "base = " << m_base << std::endl;
	m_height = positionBase1.y - positionTop1.y;
	m_hypotenuse = sqrt((m_base * m_base) + (m_height * m_height));
	m_angle = asin(m_height / m_hypotenuse);
}

void Ramp::reset()
{
	positionBase2 = { 170,550 };
	positionTop1 = { 50,460 };
}

void Ramp::drawRamp()
{
	Util::DrawLine({ basePosition },{800,basePosition.y}, { 1,1,0,1 });
	Util::DrawLine(getPositionBase1(), getPositionBase2(), { 0,1,0,1 });
	Util::DrawLine(getPositionBase1(), getPositionTop1(), { 0,1,0,1 });
	Util::DrawLine(getPositionTop1(), getPositionBase2(), { 0,1,0,1 });
}
