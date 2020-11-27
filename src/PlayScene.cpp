#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include <math.h>  

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}
	
	m_pBulletPool->Draw();
}

void PlayScene::update()
{
	
	if(!EventManager::Instance().isIMGUIActive())
	{
		updateDisplayList();
		if ((SDL_GetTicks() - bulletSpawnTimer) / 1000.0f >= delay)
		{
			//std::cout << (SDL_GetTicks() - bulletSpawnTimer) << std::endl;
			m_pBulletPool->useBulletPool();
			bulletSpawnTimer = SDL_GetTicks();
		}

		// BulletPool
		m_pBulletPool->collisionCheck(m_pJet);
		m_pBulletPool->Update();

		for(int i = 0; i < m_pBulletPool->allBullets.size(); i++)
		{
			if (m_pBulletPool->allBullets[i]->active)
			{
				m_pGravityLabel->setText("Gravity = " + std::to_string((m_pBulletPool->allBullets[i]->getRigidBody()->acceleration.y)) + " m/s^2");
				m_pVelocityLabel->setText("Velocity = " + std::to_string((m_pBulletPool->allBullets[i]->getRigidBody()->velocity.y)) + " m/s");
				m_pDelayLabel->setText("Delay = " + std::to_string((delay)) + " seconds");
				m_pNumberOfBulletsLabel->setText("Total Bullets = " + std::to_string(labelNumberOfBullets));
			}
		}
		
		
	}
}

void PlayScene::clean()
{
	removeAllChildren();
	SoundManager::Instance().unload("explosion", SOUND_SFX);
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();
	// If IMGUI is active, show mouse cursor & do not take input.
	// Sort of a Pause State
	if(EventManager::Instance().isIMGUIActive())
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
	else	//don't show mouse cursor and take mouse Input
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		m_pJet->jetMovementHandle(EventManager::Instance().getMousePosition());
	}
	
	// handle player movement with GameController
	/*if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}*/


	// handle player movement if no Game Controllers found
	/*if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}
	}
	*/

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

}

void PlayScene::start()
{
	//Set Background Tatooine
	m_pBackground = new Background("../Assets/textures/bkg1.jpg","background_playscene");
	addChild(m_pBackground);
	
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Jet
	m_pJet = new Jet();
	addChild(m_pJet);

	SoundManager::Instance().setSoundVolume(80);
	
	// Reset Button
	//m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON,{50,50},false);
	//m_pBackButton->addEventListener(CLICK, [&]()-> void
	//{
	//	m_pBackButton->setActive(false);
	//	TheGame::Instance()->changeSceneState(START_SCENE);
	//});
	//m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	//{
	//		m_pBackButton->setAlpha(128);
	//});
	//m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	//{
	//		m_pBackButton->setAlpha(255);
	//});
	//addChild(m_pBackButton);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) to Pause and invoke Simulation Control Menu", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 25.0f);
	m_pInstructionsLabel->setColour({255,255,255,255});
	addChild(m_pInstructionsLabel);

	m_pGravityLabel = new Label("", "digi", 20, { 0, 0, 0,255 });
	m_pGravityLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 100.0f);
	addChild(m_pGravityLabel);

	m_pVelocityLabel = new Label("", "digi", 20, { 0, 0, 0,255 });
	m_pVelocityLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.75f, 100.0f);
	addChild(m_pVelocityLabel);

	m_pDelayLabel = new Label("", "digi", 20, { 0, 0, 0,255 });
	m_pDelayLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 125.0f);
	addChild(m_pDelayLabel);

	m_pNumberOfBulletsLabel = new Label("", "digi", 20, { 0, 0, 0,255 });
	m_pNumberOfBulletsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.75f, 125.0f);
	addChild(m_pNumberOfBulletsLabel);
	
	std::cout << "Play Scene DisplayList Before Bullet Pool count = " << PlayScene::getDisplayList().size() << std::endl;
	// Bullet Pool
	m_pBulletPool = new BulletPool(10);

}

void PlayScene::lockOnTarget()
{

	
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Paused - Simulation Control Menu", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	//if(ImGui::Button("My Button"))
	//{
	//	std::cout << "My Button Pressed" << std::endl;
	//}

	ImGui::Text("Press the backtick (`) to Resume Simulation");
	
	ImGui::Separator();

	//ImGui::DragFloat("Gravity",m_IMGUI_gravity, 0.5f, 5.0f, 25.0f);
	
	static float gravity = { 9.8f };
	if (ImGui::SliderFloat("Gravity (m/s^2)", &gravity, 0.0f, 100.0f))
	{
		for(int i = 0; i < m_pBulletPool->allBullets.size();i++)
		{
			m_pBulletPool->allBullets[i]->getRigidBody()->acceleration.y = gravity;
		}
	}

	static float velocity = { 0.0f };
	if (ImGui::SliderFloat("Current Velocity (m/s)", &velocity, 0.0f, 30.0f))
	{
		for (int i = 0; i < m_pBulletPool->allBullets.size(); i++)
		{
			m_pBulletPool->allBullets[i]->getRigidBody()->velocity.y = velocity;
		}
	}
	
	static float i_delay = { 400.0f };
	if (ImGui::SliderFloat("Bullet Spawn Delay (ms)", &i_delay, 100.0f, 5000.0f))
	{
		delay = (i_delay * 0.001f);
	}

	static int numberOfBullets = { 10 };
	if (ImGui::SliderInt("Number of Bullets", &numberOfBullets, 2, 25))
	{
		m_pBulletPool->bulletPoolResize(numberOfBullets);
		labelNumberOfBullets = numberOfBullets;
	}
	
	ImGui::Separator();
	
	if (ImGui::Button("Reset"))
	{
		reset = true;
	}

	if (ImGui::Button("Back"))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}
	
	if(reset)
	{
		gravity = 9.8f;
		for (int i = 0; i < m_pBulletPool->allBullets.size(); i++)
		{
			m_pBulletPool->allBullets[i]->getRigidBody()->acceleration.y = gravity;
		}
		velocity = 0.0f;
		for (int i = 0; i < m_pBulletPool->allBullets.size(); i++)
		{
			m_pBulletPool->allBullets[i]->getRigidBody()->velocity.y = velocity;
		}
		i_delay = 0.4f;
		delay = (i_delay * 0.001f);
		numberOfBullets = 10;
		m_pBulletPool->bulletPoolResize(numberOfBullets);
		labelNumberOfBullets = numberOfBullets;
		reset = false;
	}
	ImGui::End();
	ImGui::EndFrame();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
	
}

//void PlayScene::UseBulletPool()
//{
//	Bullet* bullet = m_pBulletPool->Spawn();
//
//	if(bullet)
//	{
//		addChild(bullet);
//	}
//	std::cout << "Play Scene DisplayList count = " << PlayScene::getDisplayList().size() << std::endl;
//	bulletSpawnTimer = SDL_GetTicks();
//}
