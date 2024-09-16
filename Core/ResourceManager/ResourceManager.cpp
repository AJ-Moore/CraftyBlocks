#include "ResourceManager.h"

namespace CraftyBlocks
{
    std::shared_ptr<Texture> CraftyBlocks::ResourceManager::GetTexture(const std::string& path)
    {
        auto found = m_textures.find(path);

        if (found != m_textures.end())
        {
            return found->second;
        }

        std::shared_ptr<Texture> texture = std::make_shared<Texture>(); 
        LoadResource(*texture, path);
        m_textures[path] = texture;
        return texture;
    }

    std::shared_ptr<Material> ResourceManager::GetMaterial(const std::string& path)
    {
        auto found = m_materials.find(path);

        if (found != m_materials.end())
        {
            return found->second;
        }

        std::shared_ptr<Material> material = std::make_shared<Material>();
        LoadResource(*material, path);
        m_materials[path] = material;
        return material;
    }

    void ResourceManager::LoadResource(Resource& resource, const std::string& path)
    {
        resource.SetResourceLocation(path);
        resource.LoadResource(path);
    }
}
