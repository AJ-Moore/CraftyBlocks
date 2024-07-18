#pragma once

#include <Core/Interface/IManagedObject.h>
#include <Util/UID.h>
#include <Core/Services/Service.h>
#include <unordered_map>
#include <Core/Services/Time.h>

namespace CraftyBlocks
{
	class Services : protected IManagedObject
	{
	public:
		static Services& GetInstance();
		static Time* GetTime() { return GetInstance().m_time.get(); }

		// Delete the methods we don't want to allow
		Services(Services const&) = delete; // Copy constructor
		void operator=(Services const&) = delete; // Copy assignment operator
	protected: 
		virtual bool Init() override;
		virtual void Unload() override;
		virtual void Update() override;
		virtual void Render() override {}

	private:
		std::unordered_map<UID, Service> m_services;

		std::unique_ptr<Time> m_time;
	};
}