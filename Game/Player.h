#pragma once

#include <Core/Entity.h>

namespace CraftyBlocks
{
	class Player : public Entity
	{
	protected:
		virtual bool Init() override final;
	};
}