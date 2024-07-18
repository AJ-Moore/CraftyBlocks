#pragma once

#include <Common.h>

namespace CraftyBlocks
{
	class Texture
	{
	public:
		Texture(std::string File);
		virtual void Bind();
	private:
		void LoadTexture();
		std::string m_textureFile;
		GLuint m_textureID;
	};
}