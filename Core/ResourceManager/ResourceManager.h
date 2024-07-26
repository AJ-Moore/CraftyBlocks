#pragma once

#include <Common.h>
#include <Renderer/Texture.h>
#include <unordered_map>
#include <memory>

namespace CraftyBlocks
{
	class ResourceManager
	{
	public: 
		std::shared_ptr<Texture> GetTexture(const std::string& path);

	private:
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
	};
}