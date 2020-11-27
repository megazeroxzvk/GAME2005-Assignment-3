#include "PlayScene2.h"
#include "Game.h"
#include "EventManager.h"
#include <math.h>  

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene2::PlayScene2()
{
	PlayScene2::start();
}

PlayScene2::~PlayScene2()
= default;

void PlayScene2::draw()
{
	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}
}

void PlayScene2::update()
{
	updateDisplayList();
	m_pBrick->CollisionCheckWithBoundingBoxes(m_pTarget);

	m_pMass->setText("Mass of Puck = " + std::to_string((m_pTarget->getMass())) + " grams");
	m_pFriction->setText("Energy Conserved = " + std::to_string((m_pTarget->getFriction())) + " %");
	m_pPointsScored->setText("Score = " + std::to_string((m_pTarget->pointsScored)) + " points");
	m_pBallVelocity->setText("Ball Velocity = " + std::to_string(Util::magnitude(m_pTarget->getRigidBody()->velocity)) + " m/s");
	m_pPaddleVelocity->setText("Paddle Velocity = " + std::to_string(Util::magnitude(m_pBrick->getRigidBody()->velocity)) + " m/s");
}

void PlayScene2::clean()
{
	removeAllChildren();
}

void PlayScene2::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isIMGUIActive())
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
	else	//don't show mouse cursor and take mouse Input
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		m_pBrick->BrickMovementHandle(EventManager::Instance().getMousePosition());
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

void PlayScene2::start()
{
	//Set Background Tatooine
	m_pBackground = new Background("../Assets/textures/bkg2.png","background_playscene2");
	addChild(m_pBackground);

	m_pTarget = new Target();
	addChild(m_pTarget);

	m_pBrick = new Brick();
	addChild(m_pBrick);
	
	// Set GUI Title
	m_guiTitle = "Play Scene";

	SoundManager::Instance().setSoundVolume(80);
	SoundManager::Instance().load("../Assets/audio/paddlehit.wav", "paddlehit", SOUND_SFX);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) for Physics Simulation Control", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 565.0f);
	addChild(m_pInstructionsLabel);

	m_pMass = new Label("", "digi", 20, { 0, 0, 0,255 }, { 0,0 }, TTF_STYLE_NORMAL, false);
	m_pMass->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.65f, 25.0f);
	addChild(m_pMass);

	m_pFriction = new Label("", "digi", 20, { 0, 0, 0,255 }, { 0,0 }, TTF_STYLE_NORMAL, false);
	m_pFriction->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.65f, 50.0f);
	addChild(m_pFriction);

	m_pBallVelocity = new Label("", "digi", 20, { 0, 0, 0,255 }, { 0,0 }, TTF_STYLE_NORMAL, false);
	m_pBallVelocity->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.65f, 75.0f);
	addChild(m_pBallVelocity);

	m_pPaddleVelocity = new Label("", "digi", 20, { 0, 0, 0,255 }, { 0,0 }, TTF_STYLE_NORMAL, false);
	m_pPaddleVelocity->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.65f, 100.0f);
	addChild(m_pPaddleVelocity);

	m_pPointsScored = new Label("", "digi", 30, { 0, 0, 0,255 },{0,0}, TTF_STYLE_BOLD,false);
	m_pPointsScored->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.07f, 25.0f);
	addChild(m_pPointsScored);

}

void PlayScene2::lockOnTarget()
{

	
}

void PlayScene2::GUI_Function() const
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

	static float mass = { 5.0f};
	if (ImGui::SliderFloat("Mass Of Puck (gms)", &mass, 0.5f, 10.0f,"%.1f"))
	{
		m_pTarget->setMass(mass);
	}

	static float friction = {0.8f };
	if (ImGui::SliderFloat("Wall Hit Energy Conserved (%)", &friction, 0.1f, 0.9f,"%.2f"))
	{
		m_pTarget->setFriction(friction);
	}

	static bool flipPaddle;
	ImGui::Checkbox("Flip/Rotate Paddle", &flipPaddle);
	m_pBrick->rotate = flipPaddle;
	
	if (ImGui::Button("Circle Puck"))
	{
		m_pTarget->setCollisionShape(CIRCLE);
	}

	if (ImGui::Button("Square Puck"))
	{
		m_pTarget->setCollisionShape(RECTANGLE);
	}

	ImGui::Separator();
	
	static bool debugView;
	ImGui::Checkbox("View Collision Boxes", &debugView);
	m_pTarget->debugView = debugView;
	m_pBrick->debugView = debugView;

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
		mass = 5.0f;
		m_pTarget->setMass(mass);
		friction = 0.8f;
		m_pTarget->setFriction(friction);
		debugView = false;
		flipPaddle = false;
		m_pBrick->rotate = flipPaddle;
		m_pTarget->setCollisionShape(CIRCLE);
		reset = false;
	}
	ImGui::End();
	ImGui::EndFrame();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
