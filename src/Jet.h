#pragma once
#ifndef __JET__
#define __JET__

#include "Sprite.h"

class Jet final : public Sprite
{
public:
	Jet();
	~Jet();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;


	//Movement of Jet
	void jetMovementHandle(glm::vec2 mousePos);

	//Bounding box updates
	void boundingBox1Update();
	void boundingBox2Update();
	//void boundingBoxUpdate();
	SDL_Rect getCollisionBox1();
	SDL_Rect getCollisionBox2();

private:
	SDL_Rect boundingBox1, boundingBox2;
	//SDL_Rect boundingBox[];
};

#endif /* defined (__JET__) */
