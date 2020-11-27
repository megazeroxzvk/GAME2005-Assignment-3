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

	//if(EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	//{
	//	TheGame::Instance()->changeSceneState(PLAY_SCENE);
	//}
}

void StartScene::start()
{

	m_pBackground = new Background("../Assets/textures/start_screen.jpg", "background_startscene");
	addChild(m_pBackground);
	const SDL_Color black = {0, 0, 0, 255 };
	/*m_pStartLabel = new Label("START SCENE", "Consolas", 80, white, glm::vec2(400.0f, 40.0f));
	m_pStartLabel->setParent(this);
	addChild(m_pStartLabel);*/

	m_pGameTitle = new Label("Physics Mini Games", "gamer", 80, black, glm::vec2(400.0f, 60.0f));
	m_pGameTitle->setParent(this);
	addChild(m_pGameTitle);

	m_pTeamMember1 = new Label("VINEET KUMAR - 101273532", "Consolas", 40, black, glm::vec2(300.0f, 250.0f));
	m_pTeamMember1->setParent(this);
	addChild(m_pTeamMember1);

	m_pTeamMember2 = new Label("RAQUEL ANDRE DA COSTA - 101247804", "Consolas", 40, black, glm::vec2(400.0f, 300.0f));
	m_pTeamMember2->setParent(this);
	addChild(m_pTeamMember2);

	
	//Assignment 3-------
	// Start Button - Falling of Bullets
	m_pStartButton_Scene1 = new Button("../Assets/textures/start_scene1.png", "startButtonScene1", START_BUTTON, { 125.0f, 500.0f },false);
	m_pStartButton_Scene1->addEventListener(CLICK, [&]()-> void
		{
			m_pStartButton_Scene1->setActive(false);
			TheGame::Instance()->changeSceneState(PLAY_SCENE);
		});

	m_pStartButton_Scene1->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pStartButton_Scene1->setAlpha(128);
		});

	m_pStartButton_Scene1->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pStartButton_Scene1->setAlpha(255);
		});
	addChild(m_pStartButton_Scene1);

	// Start Button - Air Hockey
	m_pStartButton_Scene2 = new Button("../Assets/textures/start_scene2.png", "startButtonScene2", START_BUTTON2, { 525.0f,500.0f }, false);

	m_pStartButton_Scene2->addEventListener(CLICK, [&]()-> void
		{
			m_pStartButton_Scene2->setActive(false);
			TheGame::Instance()->changeSceneState(PLAY_SCENE2);
		});

	m_pStartButton_Scene2->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pStartButton_Scene2->setAlpha(128);
		});

	m_pStartButton_Scene2->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pStartButton_Scene2->setAlpha(255);
		});
	addChild(m_pStartButton_Scene2);
}

