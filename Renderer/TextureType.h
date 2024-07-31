#pragma once

namespace CraftyBlocks
{
	enum class TextureType
	{
		None,
		Diffuse,
		Specular,
		Ambient,
		Emissive,
		Height,
		Normal,
		Shininess,
		Opacity,
		Displacement,
		Lightmap,
		Reflection,
		BaseColour,
		NormalCamera,
		EmissionColour,
		Metalness,
		Roughness,
		AmbientOcclusion,
		Unknown
	};

	enum class TextureBindType {
		TEXTURE_2D,
		TEXTURE_ARRAY_2D,
		CUBE_MAP,
		COLOUR,
		NONE
	};

	class TextureData
	{
	public:
		TextureData() = delete;
		TextureData(TextureType type);
		TextureData(TextureType type, std::string path);
		TextureData(TextureType type, std::string path, std::string shaderVar);
		std::string GetName();
		std::string GetPath() const { return m_path; }
		std::string GetShaderVariable() const { return m_shaderVariable; }
		TextureType GetTextureType() const { return m_textureType; }
		static std::string GetStringTypeForBindType(TextureBindType bindType);

	private:
		static const std::unordered_map<std::string, TextureType> m_textureLookup;
		static const std::unordered_map<std::string, TextureBindType> m_textureBindLookup;
		std::string m_shaderVariable;
		std::string m_path;
		TextureType m_textureType;
	};
}