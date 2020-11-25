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
	}

	if((SDL_GetTicks() - bulletSpawnTimer)/1000.0f >= delay)
	{
		m_pBulletPool->useBulletPool();
		bulletSpawnTimer = SDL_GetTicks();
	}

	// BulletPool
	m_pBulletPool->collisionCheck(m_pJet);
	m_pBulletPool->Update();
	
}

void PlayScene::clean()
{
	removeAllChildren();
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

	
	
	// Throw Button
	m_pThrowButton = new Button("../Assets/textures/throwbutton.png", "throwbutton", BACK_BUTTON);
	m_pThrowButton->getTransform()->position = glm::vec2(700.0f, 50.0f);
	m_pThrowButton->addEventListener(CLICK, [&]()-> void
	{
		m_pThrowButton->setActive(false);
		if(!m_pLootbox->getReachedLocation())
		{
			m_pRamp->setNecessaryValues();
			m_pLootbox->setNecessaryValues(m_pRamp->m_getAngle(), m_pRamp->m_getHypotenuse());
			m_pLootbox->startSimulation = true;
		}	
	});
	
	m_pThrowButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pThrowButton->setAlpha(128);
	});

	m_pThrowButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pThrowButton->setAlpha(255);
	});
	addChild(m_pThrowButton);

	// Reset Button
	m_pResetButton = new Button("../Assets/textures/resetbutton.png", "resetbutton", NEXT_BUTTON);
	m_pResetButton->getTransform()->position = glm::vec2(700.0f, 110.0f);
	m_pResetButton->addEventListener(CLICK, [&]()-> void
	{
		m_pResetButton->setActive(false);
		m_pLootbox->startSimulation = false;
		m_pRamp->reset();
		m_pLootbox->reset(m_pRamp->getPositionTop1().x, m_pRamp->getPositionTop1().y - m_pLootbox->getHeight());
		reset = true;	
	});
	m_pResetButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pResetButton->setAlpha(128);
	});
	m_pResetButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pResetButton->setAlpha(255);
	});
	addChild(m_pResetButton);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) for Simulation Control Menu", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 25.0f);
	m_pInstructionsLabel->setColour({255,255,255,255});
	addChild(m_pInstructionsLabel);

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
	
	ImGui::Begin("Physics Simulation Modifiers", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	//if(ImGui::Button("My Button"))
	//{
	//	std::cout << "My Button Pressed" << std::endl;
	//}

	//ImGui::Separator();

	//ImGui::DragFloat("Gravity",m_IMGUI_gravity, 0.5f, 5.0f, 25.0f);
	
	static float width = { 4.0f};
	if (ImGui::DragFloat("Ramp Width (in meters)", &width, 4.0f, 15.0f))
	{
		m_pRamp->setPositionBase2({ m_pRamp->getPositionBase1().x + (width * SCALE),m_pRamp->getPositionBase2().y });
		m_pLootbox->startSimulation = false;
		//m_pLootbox->reset(m_pRamp->getPositionTop1().x, m_pRamp->getPositionTop1().y - m_pLootbox->getHeight());
		m_pLootbox->resetPosition(m_pRamp->getPositionTop1().x, m_pRamp->getPositionTop1().y - m_pLootbox->getHeight());
	}

	static float height = { 3.0f };
	if (ImGui::SliderFloat("Ramp Height (in meters)", &height, 3.0f, 15.0f))
	{
		m_pRamp->setPositionTop1({ m_pRamp->getPositionTop1().x, m_pRamp->getPositionBase1().y - (height * SCALE) });
		m_pLootbox->startSimulation = false;
		//m_pLootbox->reset(m_pRamp->getPositionTop1().x, m_pRamp->getPositionTop1().y - m_pLootbox->getHeight());
		m_pLootbox->resetPosition(m_pRamp->getPositionTop1().x, m_pRamp->getPositionTop1().y - m_pLootbox->getHeight());
	}

	static float mass = { 12.8f };
	if (ImGui::SliderFloat("Mass (in Kilograms)", &mass, 10.0f, 20.0f))
	{
		m_pLootbox->startSimulation = false;
		//m_pLootbox->reset(m_pRamp->getPositionTop1().x, m_pRamp->getPositionTop1().y - m_pLootbox->getHeight());
		m_pLootbox->resetPosition(m_pRamp->getPositionTop1().x, m_pRamp->getPositionTop1().y - m_pLootbox->getHeight());
		m_pLootbox->setMass(mass);
	}

	static float coefficient = { 0.42 };
	if (ImGui::SliderFloat("Coefficient of Friction", &coefficient, 0.05f, 4.0f))
	{
		m_pLootbox->startSimulation = false;
		//m_pLootbox->reset(m_pRamp->getPositionTop1().x, m_pRamp->getPositionTop1().y - m_pLootbox->getHeight());
		m_pLootbox->resetPosition(m_pRamp->getPositionTop1().x, m_pRamp->getPositionTop1().y - m_pLootbox->getHeight());
		m_pLootbox->setCoefficientOfFriction(coefficient);
	}

	if(reset)
	{
		width = 4.0f;
		height = 3.0f;
		mass = 12.8f;
		coefficient = 0.42f;
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
