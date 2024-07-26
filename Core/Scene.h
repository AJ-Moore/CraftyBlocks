#pragma once

#include <memory>
#include <unordered_map>
#include <Core/GameObject.h>
#include <map>

namespace CraftyBlocks
{
	class Entity;

	class Scene : protected GameObject
	{
	public: 
		void AddEntity(const std::shared_ptr<Entity>& entity);
		void RemoveEntity(const Entity* entity);
		void UpdateRenderQueueForEntity(Entity* entity);
	protected:
		virtual bool Init() override;
		virtual void Unload() override;
		virtual void Update() override;
		virtual void Render() override;
	private:
		std::unordered_map<UID, std::shared_ptr<Entity>> m_entities;
		bool m_bDoneInit = false;

		using RenderQueue = std::map<UID, Entity*>;
		std::map<uint, RenderQueue> m_renderQueue;
	};
}