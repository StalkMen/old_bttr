// HW.h: interface for the CHW class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "hwcaps.h"
#include "../../build_engine_config.h"
#include "stats_manager.h"

class  CHW:	public pureAppActivate, public pureAppDeactivate
{
//	Functions section
public:
	CHW();
	~CHW();

	void					CreateD3D				();
	void					DestroyD3D				();
	void					CreateDevice			(HWND hw, bool move_window);

	void					DestroyDevice			();

	void					Reset					(HWND hw);

	void					selectResolution		(u32 &dwWidth, u32 &dwHeight, BOOL bWindowed);
	D3DFORMAT				selectDepthStencil		(D3DFORMAT);
	void					updateWindowProps		(HWND hw);
	BOOL					support					(D3DFORMAT fmt, DWORD type, DWORD usage);

	void	Validate(void)	{};

//	Variables section
#if defined(USE_DX11)
public:
	ID3D11Device* pDevice 				= nullptr; // render device
	ID3D11RenderTargetView*	pBaseRT 	= nullptr; // base render target
	ID3D11DepthStencilView*	pBaseZB 	= nullptr; // base depth-stencil buffer

	IDXGIFactory1* pFactory 			= nullptr;
	IDXGIAdapter1* m_pAdapter 			= nullptr;
	ID3D11DeviceContext* pContext 		= nullptr;
	IDXGISwapChain* m_pSwapChain 		= nullptr;

	CHWCaps					Caps;

	D3D_DRIVER_TYPE		m_DriverType;	//	DevT equivalent
	DXGI_SWAP_CHAIN_DESC	m_ChainDesc;	//	DevPP equivalent
	D3D_FEATURE_LEVEL		FeatureLevel;
#elif defined(USE_DX10)
public:
	IDXGIFactory* pFactory 				= nullptr;
	IDXGIAdapter* m_pAdapter 			= nullptr;
	ID3D10Device1* pDevice1 			= nullptr;
	ID3D10Device* pDevice 				= nullptr; // render device
	ID3D10Device1* pContext1 			= nullptr;
	ID3D10Device* pContext 				= nullptr;
	IDXGISwapChain* m_pSwapChain 		= nullptr;
	ID3D10RenderTargetView* pBaseRT 	= nullptr; // base render target
	ID3D10DepthStencilView* pBaseZB 	= nullptr; // base depth-stencil buffer

	CHWCaps					Caps;
	bool					m_bUsePerfhud;
	D3D10_DRIVER_TYPE		m_DriverType;	//	DevT equivalent
	DXGI_SWAP_CHAIN_DESC	m_ChainDesc;	//	DevPP equivalent
	D3D_FEATURE_LEVEL		FeatureLevel;
#endif

	stats_manager			stats_manager;

	void			UpdateViews();
	DXGI_RATIONAL	selectRefresh(u32 dwWidth, u32 dwHeight, DXGI_FORMAT fmt);

	virtual	void	OnAppActivate();
	virtual void	OnAppDeactivate();

private:
	bool					m_move_window;
};

extern ECORE_API CHW		HW;
