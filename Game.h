#pragma once

namespace CraftyBlocks
{
	class Game
	{
	public:
		void Start();
	private:
		void GameLoop();
		void Update(); 
		void Render();

		bool m_bGameRunning = false;
	};
}