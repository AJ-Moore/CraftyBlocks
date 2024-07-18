#pragma once

#include <Common.h>
#include <Renderer/Texture.h>
#include <unordered_map>

namespace CraftyBlocks
{
	class ResourceManager
	{
	public: 
		std::shared_ptr<Texture> GetTexture(const std::string& path);

	private:
		std::unordered_map<std::string, Texture> m_textures;
	};
}