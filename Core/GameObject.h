#pragma once

#include <Core/Interface/IManagedObject.h>
#include <Util/UID.h>

namespace CraftyBlocks
{
	/** Base game object class from which Entities, Components & other engine objects are derived. */
	class GameObject : protected IManagedObject
	{
	public:
		void SetName(const std::string& name) { m_name = name; }
		const std::string& GetName() const { return m_name; }
		const UID GetUID() const { return m_uid; }
	private:
		UID m_uid;
		std::string m_name = "GameObject";
	};
}