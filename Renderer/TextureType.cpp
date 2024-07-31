#include <unordered_map>
#include <Renderer/TextureType.h>
#include <Common.h>

namespace CraftyBlocks
{
	const std::unordered_map<std::string, TextureType> TextureData::m_textureLookup =
	{
		{"None", TextureType::None},
		{"Diffuse", TextureType::Diffuse},
		{"texture", TextureType::Diffuse},
		{"Specular", TextureType::Specular},
		{"Ambient", TextureType::Ambient},
		{"Emissive", TextureType::Emissive},
		{"Height", TextureType::Height},
		{"Normal", TextureType::Normal},
		{"Shininess", TextureType::Shininess},
		{"Opacity", TextureType::Opacity},
		{"Displacement", TextureType::Displacement},
		{"Lightmap", TextureType::Lightmap},
		{"Reflection", TextureType::Reflection},

		{"BaseColour", TextureType::BaseColour},
		{"NormalCamera", TextureType::NormalCamera},
		{"EmissionColour", TextureType::EmissionColour},
		{"Metalness", TextureType::Metalness},
		{"Roughness", TextureType::Roughness},
		{"AmbientOcclusion", TextureType::AmbientOcclusion}
	};

	const std::unordered_map<std::string, TextureBindType> TextureData::m_textureBindLookup =
	{
		{"None", TextureBindType::NONE},
		{"UTEXTURE_2D", TextureBindType::TEXTURE_2D},
		{"UCUBE_MAP", TextureBindType::CUBE_MAP},
		{"UTEXTURE_ARRAY_2D", TextureBindType::TEXTURE_ARRAY_2D},
		{"UCOLOUR", TextureBindType::COLOUR},
	};


	TextureData::TextureData(TextureType type)
	{
		m_textureType = type;
	}

	TextureData::TextureData(TextureType type, std::string path)
	{
		m_textureType = type;
		m_path = path;
	}

	TextureData::TextureData(TextureType type, std::string path, std::string shaderVar)
	{
		m_textureType = type;
		m_path = path;
		m_shaderVariable = shaderVar;
	}

	std::string TextureData::GetName()
	{
		for (auto texType : m_textureLookup)
		{
			if (texType.second == m_textureType)
			{
				return texType.first;
			}
		}

		return "Unknown";
	}

	std::string TextureData::GetStringTypeForBindType(TextureBindType bindType)
	{
		for (const auto& [key, value] : m_textureBindLookup)
		{
			if (value == bindType)
			{
				return key;
			}
		}

		Log(Logger::ERROR, "Key not found, missing type in bind lookup!");

		// Might seem unintuitive but in most cases it's safer to fallback to UTexture_2D
		return "UTexture_2D";
	}
}