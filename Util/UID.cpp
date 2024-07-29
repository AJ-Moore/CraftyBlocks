
#include <Util/UID.h>

namespace CraftyBlocks {

	UID::UID()
	{
		m_uid = m_uuidCount++;
	}

	const UID& UID::operator=(const UID& other) const
	{
		m_uid = other.m_uid;
		return *this;
	}

	bool UID::operator!=(const UID& rhs) const
	{
		return m_uid != rhs.m_uid;
	}

	bool UID::operator==(const UID& rhs) const
	{
		return m_uid == rhs.m_uid;
	}

	bool UID::operator<(const UID& rhs) const
	{
		return m_uid < rhs.m_uid;
	}

	bool UID::operator<=(const UID& rhs) const
	{
		return m_uid <= rhs.m_uid;
	}

	inline bool UID::operator>=(const UID& rhs) const
	{
		return m_uid >= rhs.m_uid;
	}
}
