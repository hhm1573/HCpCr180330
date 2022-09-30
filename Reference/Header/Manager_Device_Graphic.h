#pragma once
#ifndef __MANAGER_DEVICE_GRAPHIC_H__
#define __MANAGER_DEVICE_GRAPHIC_H__

#include "Base.h"

namespace Engine {
	class CManager_Device_Graphic final
		:public Engine::CBase
	{
		DECLARE_SINGLETON(CManager_Device_Graphic)

	public:
		enum WIN_MODE { MODE_FULL, MODE_WIN };

	private:
		explicit CManager_Device_Graphic(void);
		virtual ~CManager_Device_Graphic(void) = default;

	private:
		LPDIRECT3D9				m_pSDK = nullptr;
		LPDIRECT3DDEVICE9		m_pGraphic_Device = nullptr;
	private:
		Engine::My_uInt			m_wSizeX = 0;
		Engine::My_uInt			m_wSizeY = 0;

	public:
		HRESULT							Initialize_Graphic_Device(WIN_MODE eMode, HWND hWnd, const Engine::My_uInt& wSizeX, const Engine::My_uInt& wSizeY);
	private:
		HRESULT							SetParameters(D3DPRESENT_PARAMETERS& d3dpp, WIN_MODE eMode, HWND hWnd, const Engine::My_uInt& wSizeX, const Engine::My_uInt& wSizeY);

	public:
		LPDIRECT3DDEVICE9				Get_Graphic_Device(void) { return m_pGraphic_Device; }

	protected:
		virtual Engine::My_uLong		ReleaseMemory(void) override;
	};
}

#endif // !__MANAGER_DEVICE_GRAPHIC_H__