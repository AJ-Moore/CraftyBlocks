#include "Game.h"
#include <Common.h>

namespace CraftyBlocks
{
	void Game::Start()
	{
		if (m_bGameRunning)
		{
			Logger::Log(Logger::WARN, "Game already running!");
			return;
		}
	}

	void Game::GameLoop()
	{
		Update();
		Render();
	}

	void Game::Update()
	{
	}

	void Game::Render()
	{
	}
}
