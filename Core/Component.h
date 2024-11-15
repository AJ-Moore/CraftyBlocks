#pragma once

#include <Core/GameObject.h>
#include <Util/Event.h>

namespace CraftyBlocks
{
	class Transform;
	class Entity;

	class Component : public GameObject
	{
		friend class Entity;
	public:
		std::shared_ptr<Transform> GetTransform();
	protected:
		virtual bool Init() override {}
		virtual void Unload() override {}
		virtual void Update() override {}
		virtual void Render() override {}

		virtual void OnTransformChanged() {}
		Event<Component*> OnInitialised;
	private:
		Entity* m_parent;
		bool m_bDoneInit = false;
	};
}