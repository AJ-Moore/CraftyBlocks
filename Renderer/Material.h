#pragma once 

#include <Renderer/Shader.h>
#include <Renderer/Texture.h>
#include <memory>

namespace CraftyBlocks
{
	class Material : public Resource
	{
	public: 
		void Bind();

		Shader* GetShader() const;
		Texture* GetTexture() const;
	protected: 
		virtual void LoadResource(std::string path) override;
	private:
		std::unique_ptr<Shader> m_shader;
		std::shared_ptr<Texture> m_texture;
		std::unique_ptr<Shader> m_shader;
	};
}