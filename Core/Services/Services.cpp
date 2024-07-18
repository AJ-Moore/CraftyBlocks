#include "Services.h"

namespace CraftyBlocks
{
    Services& Services::GetInstance()
    {
        static Services instance;
        return instance;
    }

    bool Services::Init()
    {
        return false;
    }

    void Services::Unload()
    {
    }
    void Services::Update()
    {
    }
}
