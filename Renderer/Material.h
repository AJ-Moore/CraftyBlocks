#pragma once 

#include <Renderer/Shader.h>
#include <memory>

namespace CraftyBlocks
{
	class Material
	{
	public: 
		Shader* GetShader() const;
		Texture* GetTexture() const;
	private:
		std::unique_ptr<Shader> m_shader;
	};
}