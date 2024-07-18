#include "Material.h"

namespace CraftyBlocks
{
    Shader* Material::GetShader() const
    {
        return m_shader.get();
    }
}
