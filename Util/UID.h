#pragma once 
#include <Common.h>
#include <filesystem>
#include <cstddef>

namespace CraftyBlocks {
	class UID {
	public:
		UID();
		uint64 GetID() const { return m_uid; }

		const UID& operator=(const UID& other) const;
		bool operator!=(const UID& rhs) const;
		bool operator==(const UID& rhs) const;

		// <,<=,>= required for sorting.
		bool operator< (const UID& rhs) const;
		bool operator<=(const UID& rhs) const;
		bool operator>=(const UID& rhs) const;

	private:
		mutable uint64 m_uid;
		static uint64 m_uuidCount;
	};
}

template <>
struct std::hash<CraftyBlocks::UID>
{
	std::size_t operator()(const CraftyBlocks::UID& k) const
	{
		return hash<uint64>()(k.GetID());
	}
};
