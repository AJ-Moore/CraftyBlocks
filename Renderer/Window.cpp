#include "Window.h"

void CraftyBlocks::Window::Init()
{
#ifdef RENDERER_OPENGL
#if __APPLE__
	const char* glsl_version = "#version 150";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
	const char* glsl_version = "#version 460";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
#endif
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif 

	m_sdlWindow = SDL_CreateWindow(m_windowCaption.c_str(), m_windowRect.X, m_windowRect.Y, m_windowRect.Width, m_windowRect.Height, SDL_WINDOW_OPENGL);
	m_glContext = SDL_GL_CreateContext(m_sdlWindow);
}

void CraftyBlocks::Window::Render()
{
	for (auto& viewport : m_viewports)
	{
#ifdef RENDERER_OPENGL
		SDL_GL_MakeCurrent(m_sdlWindow, m_glContext);
#endif
		SDL_GL_SwapWindow(m_sdlWindow);
		viewport->Render();
	}
}
