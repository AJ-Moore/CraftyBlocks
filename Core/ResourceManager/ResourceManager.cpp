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
        texture->SetResourceLocation(path);
        texture->LoadResource(path);
        m_textures[path] = texture;
        return texture;
    }
}
