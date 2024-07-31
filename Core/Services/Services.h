#pragma once

#include <Core/Interface/IManagedObject.h>
#include <Util/UID.h>
#include <Core/Services/Service.h>
#include <unordered_map>
#include <Core/Services/Time.h>
#include <Renderer/Renderer.h>
#include <Renderer/Window.h>

namespace CraftyBlocks
{
	class Services : protected IManagedObject
	{
		friend class Game;
	public:
		Services() = default;
		static Services& GetInstance();
		static Time* GetTime() { return GetInstance().m_time.get(); }
		static Renderer* GetRenderer() { return GetInstance().m_renderer.get(); }

		// Delete the methods we don't want to allow
		Services(Services const&) = delete; // Copy constructor
		void operator=(Services const&) = delete; // Copy assignment operator
	protected: 
		virtual bool Init() override;
		virtual void Unload() override;
		virtual void Update() override;
		virtual void Render() override;

	private:
		std::unordered_map<UID, std::shared_ptr<Service>> m_services;

		std::shared_ptr<Time> m_time;
		std::shared_ptr<Renderer> m_renderer;
		std::shared_ptr<Window> m_window;
	};
}