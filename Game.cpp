#include "Game.h"
#include <Common.h>
#include <Core/Services/Services.h>

namespace CraftyBlocks
{
	void Game::Start()
	{
		if (m_bGameRunning)
		{
			Logger::Log(Logger::WARN, "Game already running!");
			return;
		}

		Init();
		GameLoop();
		Unload(); 
	}

	void Game::GameLoop()
	{
		while (m_bGameRunning)
		{
			Update();
			Render();
		}
	}

	void Game::Init()
	{
		Services::GetInstance().Init();
	}

	void Game::Unload()
	{
		Services::GetInstance().Unload();
	}

	void Game::Update()
	{
		SDL_PumpEvents();
		Services::GetInstance().Init();
	}

	void Game::Render()
	{
		Services::GetInstance().Render();
	}
}
