#pragma once
#include "Base.h"

namespace Engine {
	class ENGINE_DLL CGraphicDev final
		:public Engine::CBase
	{
		DECLARE_SINGLETON(CGraphicDev)

	public:
		enum WIN_MODE { MODE_FULL, MODE_WIN };

	private:
		explicit CGraphicDev(void);
		virtual ~CGraphicDev(void) = default;

	private:
		LPDIRECT3D9				m_pSDK = nullptr;
		LPDIRECT3DDEVICE9		m_pDevice = nullptr;
		LPD3DXSPRITE			m_pSprite = nullptr;
		LPD3DXFONT				m_pFont = nullptr;
	private:
		Engine::My_uInt			m_wSizeX = 0;
		Engine::My_uInt			m_wSizeY = 0;

	public:
		HRESULT							Initialize_Graphic_Device(WIN_MODE eMode, HWND hWnd, const Engine::My_uInt& wSizeX, const Engine::My_uInt& wSizeY);
	private:
		HRESULT							SetParameters(D3DPRESENT_PARAMETERS& d3dpp, WIN_MODE eMode, HWND hWnd, const Engine::My_uInt& wSizeX, const Engine::My_uInt& wSizeY);

	public:
		LPDIRECT3DDEVICE9				Get_Graphic_Device(void) { return m_pDevice; }
		LPD3DXSPRITE					Get_Sprite() { return m_pSprite; }
		LPD3DXFONT						Get_Font() { return m_pFont; }

	protected:
		virtual Engine::My_uLong		ReleaseMemory(void) override;
	};
}