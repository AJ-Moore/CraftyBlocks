#include "Scene.h"
#include <Common.h>
#include <Core/Entity.h>

namespace CraftyBlocks
{
	void Scene::AddEntity(const std::shared_ptr<Entity>& entity)
	{
		if (m_entities.find(entity->GetUID()) == m_entities.end())
		{
			Logger::Log(Logger::WARN, "Entity has already been added to scene.");
			return;
		}

		m_entities.insert(std::make_pair(entity->GetUID(), entity));
	}

	void Scene::RemoveEntity(const Entity* entity)
	{
		if (m_entities.find(entity->GetUID()) == m_entities.end())
		{
			Logger::Log(Logger::WARN, "Unable to remove entity not found in scene.");
			return;
		}

		m_entities.erase(entity->GetUID());
	}

	void Scene::UpdateRenderQueueForEntity(Entity* entity)
	{
		if (entity == nullptr || m_renderQueue.empty())
		{
			return;
		}

		if (entity->m_currentsRenderPriority == entity->m_renderPriority)
		{
			return;
		}

		const auto& queue = m_renderQueue.find(entity->m_currentsRenderPriority);

		if (queue != m_renderQueue.end())
		{
			auto iter = queue->second.find(entity->GetUID());

			if (iter != queue->second.end())
			{
				m_renderQueue[entity->GetRenderPriority()].insert(std::make_pair(entity->GetUID(), entity));
				queue->second.erase(entity->GetUID());
				entity->m_currentsRenderPriority = entity->m_renderPriority;
			}
		}
		else
		{
			Log(Logger::WARN, "Unable to update render queue.");
		}
	}

	bool Scene::Init()
	{
		return false;
	}

	void Scene::Unload()
	{
	}

	void Scene::Render()
	{
	}
}
