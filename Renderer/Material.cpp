#include "Material.h"

namespace CraftyBlocks
{
    void Material::Bind()
    {
    }

    Shader* Material::GetShader() const
    {
        return m_shader.get();
    }

    Texture* Material::GetTexture() const
    {
        return nullptr;
    }
}
