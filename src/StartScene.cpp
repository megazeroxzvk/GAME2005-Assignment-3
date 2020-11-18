#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

StartScene::StartScene()
{
	StartScene::start();
}

StartScene::~StartScene()
= default;

void StartScene::draw()
{
	drawDisplayList();
}

void StartScene::update()
{
	updateDisplayList();
}

void StartScene::clean()
{
	removeAllChildren();
}

void StartScene::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	}
}

void StartScene::start()
{

	m_pBackground = new Background("../Assets/textures/start_screen.jpg", "background_startscene");
	addChild(m_pBackground);
	const SDL_Color cyan = {125, 200, 255, 255 };
	/*m_pStartLabel = new Label("START SCENE", "Consolas", 80, white, glm::vec2(400.0f, 40.0f));
	m_pStartLabel->setParent(this);
	addChild(m_pStartLabel);*/

	m_pAngryWookie = new Label("FALL OF LOOTBOX", "gamer", 80, cyan, glm::vec2(400.0f, 60.0f));
	m_pAngryWookie->setParent(this);
	addChild(m_pAngryWookie);

	m_pTeamMember1 = new Label("VINEET KUMAR - 101273532", "Consolas", 40, cyan, glm::vec2(300.0f, 250.0f));
	m_pTeamMember1->setParent(this);
	addChild(m_pTeamMember1);

	m_pTeamMember2 = new Label("RAQUEL ANDRE DA COSTA - 101247804", "Consolas", 40, cyan, glm::vec2(400.0f, 300.0f));
	m_pTeamMember2->setParent(this);
	addChild(m_pTeamMember2);

	/*m_pInstructionsLabel = new Label("Press 1 to Play", "Consolas", 40, white, glm::vec2(400.0f, 120.0f));
	m_pInstructionsLabel->setParent(this);
	addChild(m_pInstructionsLabel);*/

	//Assignment 3-------
	// Start Button - Falling of Bullets
	m_pStartButton_1 = new Button("../Assets/textures/startButton.png", "startButton1", START_BUTTON, { 0.0f,0.0f }, true);
	m_pStartButton_1->getTransform()->position = glm::vec2(400.0f, 500.0f); 

	m_pStartButton_1->addEventListener(CLICK, [&]()-> void
	{
		m_pStartButton_1->setActive(false);
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	});
	
	m_pStartButton_1->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pStartButton_1->setAlpha(128);
	});

	m_pStartButton_1->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pStartButton_1->setAlpha(255);
	});
	addChild(m_pStartButton_1);

	
	// Start Button - DX Ball
	m_pStartButton_2 = new Button("../Assets/textures/throwbutton.png", "startButton2", START_BUTTON, { 0.0f,0.0f }, true);
	m_pStartButton_2->getTransform()->position = glm::vec2(400.0f, 300.0f);

	m_pStartButton_2->addEventListener(CLICK, [&]()-> void
		{
			m_pStartButton_2->setActive(false);
			TheGame::Instance()->changeSceneState(PLAY_SCENE2);
		});

	m_pStartButton_2->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pStartButton_2->setAlpha(128);
		});

	m_pStartButton_2->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pStartButton_2->setAlpha(255);
		});
	addChild(m_pStartButton_2);

}

