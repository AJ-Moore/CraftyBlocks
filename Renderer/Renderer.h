#pragma once

#include <Core/Services/Service.h>
#include <Renderer/Camera.h>
#include <Renderer/Lighting/Light.h>

namespace CraftyBlocks
{
	class Renderer : public Service
	{
	public: 
		std::shared_ptr<Camera> GetActiveCamera() const { return m_camera; }
		bool IsShadowPass() { return m_bIsShadowPass; }
		Light* GetActiveLight() { }
	protected:
		virtual bool Init();
		virtual void Unload();
		virtual void Update();
		virtual void Render();
	private:
		std::shared_ptr<Camera> m_camera;
		bool m_bIsShadowPass = false;
		Light* m_activeLight = nullptr;
	};
}