#pragma once

#include <Core/Interface/IManagedObject.h>
#include <Util/Event.h>

namespace CraftyBlocks
{
	class Component : public GameObject
	{
		friend class Entity;
	protected:
		virtual bool Init() override;
		virtual void Unload() override;
		virtual void Update() override;
		virtual void Render() override;

		virtual void OnTransformChanged();
		Event<Component*> OnInitialised;
	private:
		Entity* m_parent;
		bool m_bDoneInit = false;
	};
}