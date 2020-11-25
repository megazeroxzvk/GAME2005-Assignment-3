#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Background.h"
#include "Bullet.h"
#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Ship.h"
#include "Wookie.h"
#include "Label.h"
#include "Ramp.h"
#include "Lootbox.h"
#include "Jet.h"
#include "BulletPool.h"
#include "ThermalDetonator.h"
#include "StormTroopers.h"

static int slider_position;
static float slider_speed;
static float slider_angle;


class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

	mutable bool m_locked;
	void lockOnTarget();
	
private:
	// IMGUI Function
	void GUI_Function() const;
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	// Sprites
	Ramp* m_pRamp;
	Lootbox* m_pLootbox;
	Jet* m_pJet;

	// Bullet / BulletPool
	BulletPool* m_pBulletPool;
	const float delay = 0.4f;
	float bulletSpawnTimer = 0;

	
	// UI Items
	Button* m_pThrowButton;
	Button* m_pResetButton;
	Label* m_pInstructionsLabel;

	//IMGUI adjusters
	float* m_IMGUI_gravity;


	mutable bool reset = false;

	

	// Background
	Background* m_pBackground;
};

#endif /* defined (__PLAY_SCENE__) */