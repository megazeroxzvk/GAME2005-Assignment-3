#pragma once
#ifndef __BACKGROUND__
#define __BACKGROUND__

#include "Sprite.h"

class Background : public Sprite
{
public:
	Background();
	Background(std::string m_path,std::string m_name);
	~Background();

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

private:
	std::string m_path, m_name;
};

#endif		/* defined (__BACKGROUND__) */