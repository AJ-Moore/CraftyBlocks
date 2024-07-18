#pragma once

#include <memory>
#include <Util/State.h>
#include <Core/GameObject.h>

namespace CraftyBlocks
{
	class StateMachine : public GameObject
	{
	private: 
		std::shared_ptr<State> m_currentState;
	};
}