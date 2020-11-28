// DEVICE_HW::XRAY::HW.h: interface for the CHW class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "hwcaps.h"
#include "../../build_engine_config.h"
#include "stats_manager.h"

namespace DEVICE_HW
{
	namespace XRAY
	{
		class  CHW : public pureAppActivate, public pureAppDeactivate
		{
			//	Functions section
		public:
			CHW();
			~CHW();

			void					CreateD3D();
			void					DestroyD3D();
			void					CreateDevice(HWND hw, bool move_window);

			void					DestroyDevice();

			void					Reset(HWND hw);

			void					selectResolution(u32& dwWidth, u32& dwHeight, BOOL bWindowed);
			D3DFORMAT				selectDepthStencil(D3DFORMAT);
			void					updateWindowProps(HWND hw);
			BOOL					support(D3DFORMAT fmt, DWORD type, DWORD usage);

			void	Validate(void) {};

#if defined(USE_DX10)
			ID3D10Device* pRenderDevice = nullptr; // Устройство DX (Одинаковая функция, разные структуры, между DX10-11)
			ID3D10Device* pRenderContext = nullptr;
#else
			ID3D11Device* pRenderDevice = nullptr; // Устройство DX (Одинаковая функция, разные структуры, между DX10-11)
			ID3D11DeviceContext* pRenderContext = nullptr;
#endif

			//	Variables section
#if defined(USE_DX11)
		public:
			ID3D11RenderTargetView* pBaseRT = nullptr; // base render target
			ID3D11DepthStencilView* pBaseZB = nullptr; // base depth-stencil buffer

			IDXGIFactory1* pFactory = nullptr;
			IDXGIAdapter1* m_pAdapter = nullptr;
			IDXGISwapChain* m_pSwapChain = nullptr;

			D3D_DRIVER_TYPE		m_DriverType;	//	DevT equivalent
			DXGI_SWAP_CHAIN_DESC	m_ChainDesc;	//	DevPP equivalent
#elif defined(USE_DX10)
		public:
			IDXGIFactory* pFactory = nullptr;
			IDXGIAdapter* m_pAdapter = nullptr;
			ID3D10Device1* pDevice1 = nullptr;
			ID3D10Device1* pContext1 = nullptr;
			IDXGISwapChain* m_pSwapChain = nullptr;
			ID3D10RenderTargetView* pBaseRT = nullptr; // base render target
			ID3D10DepthStencilView* pBaseZB = nullptr; // base depth-stencil buffer

			bool					m_bUsePerfhud;
			D3D10_DRIVER_TYPE		m_DriverType;	//	DevT equivalent
			DXGI_SWAP_CHAIN_DESC	m_ChainDesc;	//	DevPP equivalent
#endif

			CHWCaps					Caps;
			D3D_FEATURE_LEVEL		FeatureLevel;
			stats_manager			stats_manager;

			void					UpdateViews();
			DXGI_RATIONAL			selectRefresh(u32 dwWidth, u32 dwHeight, DXGI_FORMAT fmt);

			virtual	void			OnAppActivate();
			virtual void			OnAppDeactivate();

					void			free_vid_mode_list();
					void			fill_vid_mode_list(CHW* _hw);
#ifdef USE_DX11
			bool					ComputeShadersSupported;
			bool					DoublePrecisionFloatShaderOps;
			bool					SAD4ShaderInstructions;
			bool					ExtendedDoublesShaderInstructions;
#endif

		private:
			bool					m_move_window;
		};

		extern ECORE_API CHW		HW;
	}
}