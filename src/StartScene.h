#pragma once
#ifndef __START_SCENE__
#define __START_SCENE__

#include "Background.h"
#include "Scene.h"
#include "Label.h"
#include "ship.h"
#include "Button.h"

class StartScene final : public Scene
{
public:
	StartScene();
	~StartScene();

	// Inherited via Scene
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
	
private:

	//Assignment 3:
	//Labels
	Label* m_pTeamMember1{};
	Label* m_pTeamMember2{};
	Label* m_pGameTitle{};

	Background* m_pBackground;
	
	//Buttons
	Button* m_pStartButton_Scene1;
	Button* m_pRestartButton;
	Button* m_pStartButton_Scene2;

};

#endif /* defined (__START_SCENE__) */