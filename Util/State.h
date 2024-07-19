#pragma once

#include <Core/GameObject.h>
#include <unordered_map>
#include <functional>

namespace CraftyBlocks
{
	class State;

	class Transition
	{
	public:
		Transition(std::shared_ptr<State> state, std::function<bool> func) { m_targetState = state; m_condition = func; }
	private:
		std::shared_ptr<State> m_targetState;
		std::function<bool> m_condition;
	};

	class State : public GameObject
	{
		friend class StateMachine;
	protected:
		virtual bool Init() {}
		virtual void Unload() {}
		virtual void Render() {}
		virtual void Update() {}

		virtual void OnStateEnter() = 0;
		virtual void OnStateLeave() = 0;
	private:
		std::vector<Transition> m_transitions;
	};
}