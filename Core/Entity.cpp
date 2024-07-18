#include <Core/Entity.h>

namespace CraftyBlocks
{
	bool Entity::Init()
	{
		if (m_bDoneInit)
		{
			Log(Logger::WARN, "Init called multiple times on same entity.");
			return true;
		}

		for (auto component : m_components)
		{
			std::shared_ptr<Component> comp = component.second;
			comp->Init();
			comp->m_bDoneInit = true;
		}

		m_bDoneInit = true;
		return true;
	}

	void Entity::Unload()
	{
	}

	void Entity::Update()
	{
	}

	void Entity::Render()
	{
	}

	void Entity::OnTransformChanged()
	{
		for (auto& component : m_components)
		{
			component.second->OnTransformChanged();
		}
	}

	void Entity::InitialiseComponent(std::shared_ptr<Component> component)
	{
		component->Init();
		component->m_bDoneInit = true;
		//registerCollider(component);
		component->OnInitialised.Invoke(component.get());
	}

	std::shared_ptr<Component> Entity::AddComponent(std::shared_ptr<Component> component)
	{
		if (component == nullptr)
		{
			Log(Logger::ERROR, "Trying to add null component to entity!");
			return nullptr;
		}

		component->m_parent = this;

		if (m_parentScene != nullptr)
		{
			InitialiseComponent(component);
		}
		else
		{
			m_componentsPendingInitialisation.emplace(component);
		}

		m_componentsPendingAdd.emplace(component);
		return component;
	}

	void Entity::AddChild(std::shared_ptr<Entity> child)
	{
		if (child == nullptr)
		{
			return;
		}

		m_children.insert(std::make_pair(child->GetUID(), child));
		Transform::MakeParent(child->m_transform, m_transform);

		if (m_parentScene != nullptr)
		{
			m_parentScene->AddEntity(child);
		}
		else
		{
			m_entitiesPendingAdd.emplace(child);
		}
	}

	std::shared_ptr<Entity> Entity::RemoveChild(Entity* child)
	{
		return std::shared_ptr<Entity>();
	}

	void Entity::RemoveComponent(Component* component)
	{
		if (component == nullptr)
		{
			return;
		}

		component->Unload();
		m_components.erase(component->GetUID());
	}

	void Entity::SetRenderPriority(uint value)
	{
		if (value == m_currentsRenderPriority)
		{
			return;
		}

		m_renderPriority = value;
		Scene* scene = GetParentScene();

		if (scene != nullptr)
		{
			scene->UpdateRenderQueueForEntity(this);
		}
	}
}