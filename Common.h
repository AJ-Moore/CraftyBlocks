#pragma once

#include <Util/Logger.h>
#include <Util/UID.h>

using uint64 = long int;
using uint32 = unsigned int; 
using int32 = int;
using uint = uint32;

#define RENDERER_OPENGL
//#define RENDERER_VULKEN

#if defined(RENDERER_VULKAN)
#include <vulkan/vulkan.h>
#endif

#if defined(RENDERER_OPENGL)
#include <GL/glew.h>
#endif

// SDL 
#include <SDL.h>
#include <SDL_image.h>
//#include <SDL_ttf.h>

// GLM 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_LEFT_HAND
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_CTOR_INIT