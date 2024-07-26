#pragma once 

#include <Renderer/Shader.h>
#include <Renderer/Texture.h>
#include <memory>

namespace CraftyBlocks
{
	class Material
	{
	public: 
		void Bind();

		Shader* GetShader() const;
		Texture* GetTexture() const;
	private:
		std::unique_ptr<Shader> m_shader;
		std::shared_ptr<Texture> m_texture;
	};
}