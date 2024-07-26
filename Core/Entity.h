#pragma once
#include <Common.h>
#include <Core/Interface/IManagedObject.h>
#include <Core/Scene.h>
#include <Core/GameObject.h>
#include <Core/Component.h>
#include <queue>
#include <Renderer/Transform.h>

namespace CraftyBlocks
{
	enum class URenderPriority
	{
		Opaque = 3000,
		Transparent = 4000,
	};

	enum class UCullingMask
	{
		Default = 0x1,
		Frame = 0x2,
	};

	class Entity : public GameObject
	{
		friend class Scene;
		friend class Transform;
	public:
		virtual std::shared_ptr<Component> AddComponent(std::shared_ptr<Component> Component);
		virtual void AddChild(std::shared_ptr<Entity> child);
		virtual std::shared_ptr<Entity> RemoveChild(Entity* child);
		virtual void RemoveComponent(Component* Component);

		template <class T, class... Arguments >
		std::shared_ptr<T> AddComponent(Arguments&&... _Args);

		template <typename T>
		std::vector<std::shared_ptr<T>> GetComponentsInChildren();

		template <typename T>
		std::shared_ptr<T> GetComponent();

		template <typename T>
		std::vector<std::shared_ptr<T>> GetComponents();

		std::shared_ptr<Transform> GetTransform() { return m_transform; }
		Scene* GetParentScene() const { return this->m_parentScene; }
		const std::unordered_map<UID, std::shared_ptr<Entity>>& GetChildren() const { return m_children; }

		uint GetRenderPriority() const { return m_renderPriority; }
		void SetRenderPriority(uint value);

		void SetCullingMask(uint mask) { m_cullingMask = mask; }
		uint GetCullingMask() const { return m_cullingMask; }

		void SetPhysicsMask(uint layer) { m_physicsMask = layer; }
		uint GetPhysicsMask() { return m_physicsMask; }

		void SetEnabled(bool enabled) { m_bEnabled = enabled; }
	protected:
		virtual bool Init() override;
		virtual void Unload() override;
		virtual void Update() override;
		virtual void Render() override;

		virtual void OnTransformChanged();
	private:
		void InitialiseComponent(std::shared_ptr<Component> component);

		std::unordered_map<UID, std::shared_ptr<Component>> m_components;
		std::unordered_map<UID, std::shared_ptr<Entity>> m_children;

		std::queue<std::shared_ptr<Entity>> m_entitiesPendingAdd;
		std::queue<std::shared_ptr<Component>> m_componentsPendingInitialisation;
		std::queue<std::shared_ptr<Component>> m_componentsPendingAdd;
		std::queue<std::shared_ptr<Component>>m_componentsPendingRemove;

		uint m_renderPriority = (uint)URenderPriority::Opaque;
		uint m_cullingMask = (uint)UCullingMask::Default;
		uint m_physicsMask = 0x1;
		uint m_currentsRenderPriority = (uint)URenderPriority::Opaque;

		std::shared_ptr<Transform> m_transform;
		Scene* m_parentScene = nullptr;

		bool m_bEnabled = true;
		bool m_bDoneInit = false;
	};

	template <class T, class... Arguments>
	std::shared_ptr<T> Entity::AddComponent(Arguments&&... _Args) {
		if (!std::is_base_of<Component, T>::value) {
			Log(Logger::ERROR, "Invalid type identifier.");
			return nullptr;
		}

		std::shared_ptr<T> componentToAdd = std::make_shared<T>(_Args...);
		addComponent(componentToAdd);

		return componentToAdd;
	}

	template<typename T>
	inline std::vector<std::shared_ptr<T>> Entity::GetComponentsInChildren()
	{
		std::vector<std::shared_ptr<T>> components;

		for (auto& child : m_children)
		{
			std::vector<std::shared_ptr<T>> childComponents = child.second->GetComponentsInChildren<T>();
			std::vector<std::shared_ptr<T>> currentComponents = child.second->GetComponents<T>();
			components.insert(components.end(), childComponents.begin(), childComponents.end());
			components.insert(components.end(), currentComponents.begin(), currentComponents.end());
		}

		return std::move(components);
	}

	template<typename T>
	inline std::shared_ptr<T> Entity::GetComponent()
	{
		for (std::shared_ptr<Component> component : m_components)
		{
			if (typeid(T) == typeid(*component.get()))
			{
				return std::dynamic_pointer_cast<T>(component);
			}
		}

		return nullptr;
	}

	template<typename T>
	inline std::vector<std::shared_ptr<T>> Entity::GetComponents()
	{
		std::vector<std::shared_ptr<T>> components;

		for (std::shared_ptr<Component> component : m_components)
		{
			if (typeid(T) == typeid(*component.get()))
			{
				components.push_back(std::dynamic_pointer_cast<T>(component));
			}
		}

		for (std::shared_ptr<Component> component : m_componentsPendingAdd)
		{
			if (typeid(T) == typeid(*component.get()))
			{
				components.push_back(std::dynamic_pointer_cast<T>(component));
			}
		}

		return std::move(components);
	}
}