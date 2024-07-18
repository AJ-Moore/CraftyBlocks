#pragma once

#include <Renderer/Viewport.h>
#include <unordered_set>

namespace CraftyBlocks
{
	class Window
	{
	private:
		std::unordered_set<Viewport> m_viewports;
	};
}