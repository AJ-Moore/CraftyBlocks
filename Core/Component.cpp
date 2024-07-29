#include <Core/Component.h>
#include <Renderer/Transform.h>
#include <memory>

namespace CraftyBlocks
{
    std::shared_ptr<Transform> Component::GetTransform()
    {
        if (m_parent == nullptr)
        {
            return nullptr;
        }

        return m_parent->GetTransform();
    }
}
