#pragma once

#include <Common.h>
#include <Renderer/Texture.h>
#include <Renderer/Material.h>
#include <unordered_map>
#include <memory>

namespace CraftyBlocks
{
	class ResourceManager
	{
	public: 
		std::shared_ptr<Texture> GetTexture(const std::string& path);
		std::shared_ptr<Material> GetMaterial(const std::string& path);

	private:
		void LoadResource(Resource& resource, const std::string& path);

		std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
		std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;
	};
}