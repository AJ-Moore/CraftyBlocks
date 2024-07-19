#pragma once

#include <Common.h>

namespace CraftyBlocks
{
	enum ResourceType
	{
		TEXTURE = 0x1, 
		MODEL = 0x2, 
		NONE = 0x0
	};

	class Resource
	{
		friend class ResourceManager;
	public:
		const std::string& GetResourceLocation() { return m_resourceLocation; }
	protected:
		virtual void LoadResource(std::string path) = 0;
		void SetResourceLocation(const std::string& path) { m_resourceLocation = path; }
	private:
		std::string m_resourceLocation;
	};
}