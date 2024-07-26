#include "Component.h"

std::shared_ptr<Transform> CraftyBlocks::Component::GetTransform()
{
    if (m_parent == nullptr)
    {
        return nullptr;
    }

    return m_parent->GetTransform();
}
