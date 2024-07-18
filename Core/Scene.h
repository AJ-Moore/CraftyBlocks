#pragma once

#include <memory>
#include <unordered_map>
#include <Core/Entity.h>
#include <map>

namespace CraftyBlocks
{
	class Scene : protected IManagedObject
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

		using RenderQueue = std::map<uint, Entity*>;
		std::map<uint, RenderQueue> m_renderQueue;
	};
}