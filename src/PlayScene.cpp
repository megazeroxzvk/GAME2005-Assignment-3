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
}

void PlayScene::update()
{
	updateDisplayList();
	m_pVelocityLabel->setText("Velocity = " + std::to_string(fabs(m_pLootbox->getRigidBody()->velocity.x) / SCALE) + " m/s");
	m_pAngleLabel->setText("Angle = " + std::to_string(m_pLootbox->getAngle()) + " deg");
	m_pTotalDistance->setText("Total Distance = " + std::to_string(m_pLootbox->getDistance()) + " m");
	m_pAccelerationLabel->setText("Acceleration = " + std::to_string(fabs(m_pLootbox->getRigidBody()->acceleration.y) / SCALE) + " m/s^2");
	m_pForce->setText("Force = " + std::to_string((m_pLootbox->getForce().x / SCALE)) + " N");
	m_pMass->setText("Mass = " + std::to_string(fabs(m_pLootbox->getMass())) + " Kg");
	m_pPPM->setText("Scale Value: 30 Pixels = 1 Meter");
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

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
	m_pBackground = new Background("../Assets/textures/owbkg.jpg","background_playscene");
	addChild(m_pBackground);
	
	// Set GUI Title
	m_guiTitle = "Play Scene";

	//Ramp
	m_pRamp = new Ramp();
	addChild(m_pRamp);

	//Lootbox
	m_pLootbox = new Lootbox();
	addChild(m_pLootbox);
	m_pLootbox->setPosition(m_pRamp->getPositionTop1().x, m_pRamp->getPositionTop1().y - m_pLootbox->getHeight());

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
	m_pInstructionsLabel = new Label("Press the backtick (`) for Physics Simulation Control", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 565.0f);
	addChild(m_pInstructionsLabel);

	m_pPPM = new Label("", "digi", 20, { 0,0,0,255 });
	m_pPPM->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5, 585.0f);
	addChild(m_pPPM);

	m_pVelocityLabel = new Label("", "digi", 20,{ 255, 255, 255,255});
	m_pVelocityLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.85f, 340.0f);
	addChild(m_pVelocityLabel);

	m_pAngleLabel = new Label("", "digi", 20, { 255,255,255,255 });
	m_pAngleLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.85f, 360.0f);
	addChild(m_pAngleLabel);

	m_pTotalDistance = new Label("", "digi", 20, { 255,255,255,255 });
	m_pTotalDistance->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.85f, 380.0f);
	addChild(m_pTotalDistance);

	m_pAccelerationLabel = new Label("", "digi", 20, { 255,255,255,255 });
	m_pAccelerationLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.85f, 400.0f);
	addChild(m_pAccelerationLabel);

	m_pForce = new Label("", "digi", 20, { 255,255,255,255 });
	m_pForce->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.85f, 420.0f);
	addChild(m_pForce);

	m_pMass = new Label("", "digi", 20, { 255,255,255,255 });
	m_pMass->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.85f, 440.0f);
	addChild(m_pMass);

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

	static float width = { 4.0f};
	if (ImGui::SliderFloat("Ramp Width (in meters)", &width, 4.0f, 15.0f))
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
