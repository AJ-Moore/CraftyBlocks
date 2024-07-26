#pragma once

#include <Common.h>
#include <Core/ResourceManager/Resource.h>
#include <Renderer/TextureUtils.h>
#include <vector>

namespace CraftyBlocks
{
	class Texture : public Resource
	{
		friend class ResourceManager;
	public:
		virtual void Bind();
		virtual void Bind(uint textureUnit);
	protected:
		virtual void LoadResource(std::string path);
	private:
		GLint GetTextureFormat(SDL_Surface* image);
		GLuint CreatePinkBlackTexture();
		std::vector<Colour> m_pixelData;
		GLuint m_textureID = 0;
		uint m_width = 0; 
		uint m_height = 0;
		UTextureFilter m_textureFilter = UTextureFilter::Linear;
		UTextureClamp m_textureClamp = UTextureClamp::Repeat;
		bool m_bStorePixelData = false;
		uint32 m_imageFormat = GL_RGBA;
	};
}