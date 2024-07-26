#include "Texture.h"
#include <filesystem>

void CraftyBlocks::Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void CraftyBlocks::Texture::Bind(uint textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void CraftyBlocks::Texture::LoadResource(std::string path)
{
	Log(Logger::INFO, "Loading texture [%s]", path.c_str());
	SDL_Surface* image = nullptr;
	bool bTextureLoadOK = false;

	std::filesystem::path fsPath = std::filesystem::path(path);

	if (!std::filesystem::exists(fsPath))
	{
		Log(Logger::WARN, "Unable to find texture at path [%s]", path);
		return;
	}

	try {
		image = IMG_Load(fsPath.generic_string().c_str());
		bTextureLoadOK = true;
	}
	catch (std::exception e)
	{
		Log(Logger::ERROR, "Exception was thrown whilst trying to load texture [%s].", fsPath.generic_string().c_str());
		bTextureLoadOK = false;
	}

	if (image == nullptr)
	{
		bTextureLoadOK = false;
		Log(Logger::ERROR, "Error whilst loading texture [%s].", IMG_GetError());
		// Use default texture, return 
		m_textureID = CreatePinkBlackTexture();
		return;
	}
	else
	{
		Log(Logger::VERBOSE, "Texture loaded successfully: [%s]", fsPath.generic_string().c_str());
	}

	m_imageFormat = GetTextureFormat(image);

	glTexImage2D(GL_TEXTURE_2D,
		0,//Mipmap Level
		m_imageFormat, //bit per pixel
		image->w,
		image->h,
		0,//texture border 	
		m_imageFormat,
		GL_UNSIGNED_BYTE,
		image->pixels);

	if (m_bStorePixelData)
	{
		if (m_imageFormat == GL_RGB || m_imageFormat == GL_RGBA)
		{
			m_pixelData.clear();

			int increment = (m_imageFormat == GL_RGB) ? 3 : 4;
			unsigned char* pixels = static_cast<unsigned char*>(image->pixels);

			for (int i = 0; i < image->w * image->h * increment; i += increment)
			{
				float r = pixels[i + 0];
				float g = pixels[i + 1];
				float b = pixels[i + 2];
				float a = 1;
				if (m_imageFormat == GL_RGBA)
				{
					float a = ((unsigned char*)image->pixels)[i + 3];
				}

				m_pixelData.push_back(Colour(r, g, b, a));
			}
		}
		else
		{
			Log(Logger::WARN, "Unsupported texture format.");
		}
	}

	/// Retain some useful informastion 
	m_width = image->w;
	m_height = image->h;

	SDL_FreeSurface(image);
	LogGraphicsErrors();
}

GLint CraftyBlocks::Texture::GetTextureFormat(SDL_Surface* image)
{
	if (image == nullptr)
	{
		Log(Logger::ERROR, "Unable to get texture format.");
		return GL_RGBA;
	}

	switch (image->format->BytesPerPixel)
	{
	case 1:
		return GL_RED; 
	case 2:
		if (image->format->Rmask == 0x00FF)
			return GL_RG;  
		else
			return GL_LUMINANCE_ALPHA; 
	case 3:
		if (image->format->Rmask == 0x000000FF)
			return GL_RGB;
		else
			return GL_BGR;
	case 4:
		if (image->format->Rmask == 0x000000FF)
			return GL_RGBA;
		else
			return GL_BGRA; 
	default:
		Log(Logger::ERROR, "Texture unsupported format: %s", GetResourceLocation().c_str());
		return GL_RGBA;
	}
}

GLuint CraftyBlocks::Texture::CreatePinkBlackTexture()
{
	Log(Logger::VERBOSE, "Creating pink black debug texture.");

	// Write pink/ black missing texture 64X64
	for (int i = 0; i < 16; ++i)
	{
		for (int p = 0; p < 16; ++p)
		{
			if ((p % 2 == 0))
			{
				m_pixelData.push_back((i % 2 == 0) ? Colour(0, 0, 0, 255) : Colour(255, 0, 255, 255));
			}
			else
			{
				m_pixelData.push_back((i % 2 != 0) ? Colour(0, 0, 0, 255) : Colour(255, 0, 255, 255));
			}
		}
	}

	m_textureFilter = UTextureFilter::NearestNeighbour;

	// create the surface from our pixel data!
	GLvoid* pixelDat = (GLvoid*)(&m_pixelData.front());
	int dimensions = (int)sqrt(m_pixelData.size());

	glTexImage2D(GL_TEXTURE_2D,
		0,//Mipmap Level
		GL_RGBA, //bit per pixel
		dimensions,
		dimensions,
		0,//texture border 	
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		pixelDat);

	m_width = dimensions;
	m_height = dimensions;
}
