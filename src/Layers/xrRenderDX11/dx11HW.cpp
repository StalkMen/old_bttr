// dx10HW.cpp: implementation of the DX10 specialisation of CHW.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#pragma hdrstop

#pragma warning(disable:4995)
#include <d3dx9.h>
#pragma warning(default:4995)

#include "../xrRender/HW.h"
#include "../../xrEngine/XR_IOConsole.h"
#include "../../Include/xrAPI/xrAPI.h"
#include "..\..\Layers\xrRenderDX10\StateManager\dx10SamplerStateCache.h"
#include "..\..\Layers\xrRenderDX10\StateManager\dx10StateCache.h"

void	fill_vid_mode_list			(CHW* _hw);
void	free_vid_mode_list			();

CHW			HW;

CHW::CHW() : m_move_window(true)
{
	Device.seqAppActivate.Add(this);
	Device.seqAppDeactivate.Add(this);
}

CHW::~CHW()
{
	Device.seqAppActivate.Remove(this);
	Device.seqAppDeactivate.Remove(this);
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CHW::CreateD3D()
{
	R_CHK(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&pFactory)));
	pFactory->EnumAdapters1(0, &m_pAdapter);
}

void CHW::DestroyD3D()
{
	_SHOW_REF("refCount:m_pAdapter", m_pAdapter);
	_RELEASE(m_pAdapter);
	
	_SHOW_REF("refCount:pFactory", pFactory);
	_RELEASE(pFactory);
}

extern int tbufer_renders;
extern u32 RenderThemeShaders;
extern ENGINE_API u32 g_screenmode;
extern ENGINE_API void GetMonitorResolution(u32& horizontal, u32& vertical);

void CHW::CreateDevice(HWND m_hWnd, bool move_window)
{
	m_move_window = move_window;
	CreateD3D();

	// TODO: DX10: Create appropriate initialization

	// General - select adapter and device
	BOOL  bWindowed = (g_screenmode != 2);

	m_DriverType = Caps.bForceGPU_REF ?
		D3D_DRIVER_TYPE_REFERENCE : D3D_DRIVER_TYPE_HARDWARE;

	// Display the name of video board
	DXGI_ADAPTER_DESC1 Desc;
	R_CHK(m_pAdapter->GetDesc1(&Desc));

	//	Warning: Desc.Description is wide string
	Msg("* GPU [vendor:%X]-[device:%X]: %S", Desc.VendorId, Desc.DeviceId, Desc.Description);

	Caps.id_vendor = Desc.VendorId;
	Caps.id_device = Desc.DeviceId;

	// Select back-buffer & depth-stencil format
	D3DFORMAT& fTarget = Caps.fTarget;
	D3DFORMAT& fDepth = Caps.fDepth;

	//	HACK: DX10: Embed hard target format.
	fTarget = D3DFMT_X8R8G8B8;	//	No match in DX10. D3DFMT_A8B8G8R8->DXGI_FORMAT_R8G8B8A8_UNORM
	fDepth = selectDepthStencil(fTarget);

	// Set up the presentation parameters
	DXGI_SWAP_CHAIN_DESC& sd = m_ChainDesc;
	ZeroMemory(&sd, sizeof(sd));

	selectResolution(sd.BufferDesc.Width, sd.BufferDesc.Height, bWindowed);

	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	{
		//OldSerpskiStalker
		sd.BufferCount = (tbufer_renders == 0) ? 1 : 3;
		Msg("# HW DX11. Number of buffers for prepared frames: %i", sd.BufferCount);
	}

	Msg("------------------------------------------------------------------------------------------------------------");
	Msg("# To avoid losing the exact cause of the error, this log will show the exact definition of the Shader theme.");
	Msg("# List of existing Shader themes: ");
	Msg("# 1. cop_shaders_1_6_02   = 0. !Supported on the basis of the Call of Pripyat and Call of Chernobyl.");
	Msg("# 2. old_shaders_theme892 = 1. !Supported only in modification Back to the Roots.");
	Msg("# 3. new_shaders_theme895 = 2. !Supported only in modification Back to the Roots.");
	Msg("- Theme shader: %i", RenderThemeShaders);
	Msg("----------------------------------------------------------: TouchOfRay Engine x64 OldSerpskiStalker, LVutner");

	// Multisample
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.OutputWindow = m_hWnd;
	sd.Windowed = bWindowed;

	if (bWindowed)
	{
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
	}
	else
	{
		sd.BufferDesc.RefreshRate = selectRefresh(sd.BufferDesc.Width, sd.BufferDesc.Height, sd.BufferDesc.Format);
	}

	//	Additional set up
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	UINT createDeviceFlags = 0;

#ifdef DEBUG
	if (IsDebuggerPresent())
		createDeviceFlags |= D3D_CREATE_DEVICE_DEBUG;
#endif

	constexpr D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	constexpr auto count = std::size(featureLevels);

	const auto createDevice = [&](const D3D_FEATURE_LEVEL* level, const u32 levels)
	{
		return D3D11CreateDevice(m_pAdapter, D3D_DRIVER_TYPE_UNKNOWN, // Если мы выбираем конкретный адаптер, то мы обязаны использовать D3D_DRIVER_TYPE_UNKNOWN.
			nullptr, createDeviceFlags, level, levels,
			D3D11_SDK_VERSION, &pDevice, &FeatureLevel, &pContext);
	};

	HRESULT R = createDevice(featureLevels, count);
	if (FAILED(R))
		R_CHK(createDevice(&featureLevels[1], count - 1));

	R_CHK(pFactory->CreateSwapChain(pDevice, &sd, &m_pSwapChain));

	if (FeatureLevel != D3D_FEATURE_LEVEL_11_1)
	{
		R_ASSERT(FeatureLevel == D3D_FEATURE_LEVEL_11_0);
		Msg("# DirectX11.1 is not supported by the video card, so we launch from the standard level - DirectX11.");
	}

	if (FAILED(R))
	{
		// Fatal error! Cannot create rendering device AT STARTUP !!!
		Msg("Error in loading the graphics process.\n" "CreateDevice returned 0x%08x", R);
		FlushLog();
		MessageBox(NULL, "Your video card does not support DirectX 11! The app cannot be started.", "Error!", MB_OK | MB_ICONERROR);
		TerminateProcess(GetCurrentProcess(), 0);
	};

	_SHOW_REF("* CREATE: DeviceREF:", HW.pDevice);

	//	Create render target and depth-stencil views here
	UpdateViews();

	size_t	memory = Desc.DedicatedVideoMemory;
	Msg("*     Texture memory DX11: %d M", memory / (1024 * 1024));

	updateWindowProps(m_hWnd);
	fill_vid_mode_list(this);
}

void CHW::DestroyDevice()
{
	//	Destroy state managers
	StateManager.Reset();
	RSManager.ClearStateArray();
	DSSManager.ClearStateArray();
	BSManager.ClearStateArray();
	SSManager.ClearStateArray();

	_SHOW_REF				("# refCount:pBaseZB",pBaseZB);
	_RELEASE				(pBaseZB);

	_SHOW_REF				("# refCount:pBaseRT",pBaseRT);
	_RELEASE				(pBaseRT);

	//	Must switch to windowed mode to release swap chain
	if (!m_ChainDesc.Windowed) m_pSwapChain->SetFullscreenState( FALSE, NULL);
	_SHOW_REF				("# refCount:m_pSwapChain",m_pSwapChain);
	_RELEASE				(m_pSwapChain);
	_RELEASE				(pContext);

	_SHOW_REF				("# DeviceREF:",HW.pDevice);
	_RELEASE				(HW.pDevice);

	DestroyD3D				();

	free_vid_mode_list		();
}

//////////////////////////////////////////////////////////////////////
// Resetting device
//////////////////////////////////////////////////////////////////////
void CHW::Reset (HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC &cd = m_ChainDesc;

	BOOL	bWindowed		= (g_screenmode != 2);

	cd.Windowed = bWindowed;

	m_pSwapChain->SetFullscreenState(!bWindowed, NULL);

	DXGI_MODE_DESC	&desc = m_ChainDesc.BufferDesc;

	selectResolution(desc.Width, desc.Height, bWindowed);

	if (bWindowed)
	{
		desc.RefreshRate.Numerator = 60;
		desc.RefreshRate.Denominator = 1;
	}
	else
		desc.RefreshRate = selectRefresh( desc.Width, desc.Height, desc.Format);

	CHK_DX(m_pSwapChain->ResizeTarget(&desc));

	_SHOW_REF				("# refCount:pBaseZB",pBaseZB);
	_SHOW_REF				("# refCount:pBaseRT",pBaseRT);

	_RELEASE(pBaseZB);
	_RELEASE(pBaseRT);

	CHK_DX(m_pSwapChain->ResizeBuffers(
		cd.BufferCount,
		desc.Width,
		desc.Height,
		desc.Format,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

	UpdateViews();
	updateWindowProps	(hwnd);
}

D3DFORMAT CHW::selectDepthStencil	(D3DFORMAT fTarget)
{
	// R3 hack
#pragma todo("R3 need to specify depth format")
	return D3DFMT_D24S8;
}

void CHW::selectResolution( u32 &dwWidth, u32 &dwHeight, BOOL bWindowed )
{
	fill_vid_mode_list			(this);

	if (psCurrentVidMode[0] == 0 || psCurrentVidMode[1] == 0)
		GetMonitorResolution(psCurrentVidMode[0], psCurrentVidMode[1]);

	if(bWindowed)
	{
		dwWidth		= psCurrentVidMode[0];
		dwHeight	= psCurrentVidMode[1];
	}
	else //check
	{
		string64					buff;
		xr_sprintf					(buff,sizeof(buff),"%dx%d",psCurrentVidMode[0],psCurrentVidMode[1]);

		if(_ParseItem(buff,vid_mode_token)==u32(-1)) //not found
		{ //select safe
			xr_sprintf				(buff,sizeof(buff),"vid_mode %s",vid_mode_token[0].name);
			Console->Execute		(buff);
		}

		dwWidth						= psCurrentVidMode[0];
		dwHeight					= psCurrentVidMode[1];
	}
}

DXGI_RATIONAL CHW::selectRefresh(u32 dwWidth, u32 dwHeight, DXGI_FORMAT fmt)
{
	DXGI_RATIONAL	res;

	res.Numerator = 60;
	res.Denominator = 1;

	float	CurrentFreq = 60.0f;

	if (psDeviceFlags.is(rsRefresh60hz))	
	{
		return res;
	}
	else
	{
		xr_vector<DXGI_MODE_DESC>	modes;

		IDXGIOutput *pOutput;
		m_pAdapter->EnumOutputs(0, &pOutput);
		VERIFY(pOutput);

		UINT num = 0;
		DXGI_FORMAT format = fmt;
		UINT flags         = 0;

		// Get the number of display modes available
		pOutput->GetDisplayModeList( format, flags, &num, 0);

		// Get the list of display modes
		modes.resize(num);
		pOutput->GetDisplayModeList( format, flags, &num, &modes.front());

		_RELEASE(pOutput);

		for (u32 i=0; i<num; ++i)
		{
			DXGI_MODE_DESC &desc = modes[i];

			if( (desc.Width == dwWidth) 
				&& (desc.Height == dwHeight)
				)
			{
				VERIFY(desc.RefreshRate.Denominator);
				float TempFreq = float(desc.RefreshRate.Numerator)/float(desc.RefreshRate.Denominator);
				if ( TempFreq > CurrentFreq )
				{
					CurrentFreq = TempFreq;
					res = desc.RefreshRate;
				}
			}
		}

		return res;
	}
}

void CHW::OnAppActivate()
{
	if ( m_pSwapChain && !m_ChainDesc.Windowed )
	{
		ShowWindow( m_ChainDesc.OutputWindow, SW_RESTORE );
		m_pSwapChain->SetFullscreenState((g_screenmode == 2), nullptr);
	}
}

void CHW::OnAppDeactivate()
{
	if ( m_pSwapChain && !m_ChainDesc.Windowed )
	{
		m_pSwapChain->SetFullscreenState( FALSE, NULL );
		ShowWindow( m_ChainDesc.OutputWindow, SW_MINIMIZE );
	}
}

BOOL CHW::support( D3DFORMAT fmt, DWORD type, DWORD usage)
{
	//	TODO: DX10: implement stub for this code.
	VERIFY(!"Implement CHW::support");
	return TRUE;
}

void CHW::updateWindowProps(HWND m_hWnd)
{
	BOOL	bWindowed				= (g_screenmode != 2);

	u32		dwWindowStyle			= 0;
	// Set window properties depending on what mode were in.
	if (bWindowed)		{
		if (m_move_window) {
            dwWindowStyle = WS_BORDER | WS_VISIBLE;
            if (!strstr(Core.Params, "-no_dialog_header"))
                dwWindowStyle |= WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX;
            SetWindowLong(m_hWnd, GWL_STYLE, dwWindowStyle);
			// When moving from fullscreen to windowed mode, it is important to
			// adjust the window size after recreating the device rather than
			// beforehand to ensure that you get the window size you want.  For
			// example, when switching from 640x480 fullscreen to windowed with
			// a 1000x600 window on a 1024x768 desktop, it is impossible to set
			// the window size to 1000x600 until after the display mode has
			// changed to 1024x768, because windows cannot be larger than the
			// desktop.

			RECT			m_rcWindowBounds;
			RECT				DesktopRect;

			GetClientRect		(GetDesktopWindow(), &DesktopRect);

			SetRect(			&m_rcWindowBounds, 
				(DesktopRect.right-m_ChainDesc.BufferDesc.Width)/2, 
				(DesktopRect.bottom-m_ChainDesc.BufferDesc.Height)/2, 
				(DesktopRect.right+m_ChainDesc.BufferDesc.Width)/2, 
				(DesktopRect.bottom+m_ChainDesc.BufferDesc.Height)/2);

			AdjustWindowRect		(	&m_rcWindowBounds, dwWindowStyle, FALSE );

			SetWindowPos			(	m_hWnd, 
				HWND_NOTOPMOST,	
                m_rcWindowBounds.left,
                m_rcWindowBounds.top,
				( m_rcWindowBounds.right - m_rcWindowBounds.left ),
				( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
				SWP_SHOWWINDOW|SWP_NOCOPYBITS|SWP_DRAWFRAME );
		}
	}
	else
	{
		SetWindowLong			( m_hWnd, GWL_STYLE, dwWindowStyle=(WS_POPUP|WS_VISIBLE) );
	}

	ShowCursor	(FALSE);
	SetForegroundWindow( m_hWnd );
    RECT winRect;
    GetWindowRect(m_hWnd, &winRect);
    ClipCursor(&winRect);
}


struct _uniq_mode
{
	_uniq_mode(LPCSTR v):_val(v){}
	LPCSTR _val;
	bool operator() (LPCSTR _other) {return !stricmp(_val,_other);}
};

void free_vid_mode_list()
{
	for( int i=0; vid_mode_token[i].name; i++ )
	{
		xr_free					(vid_mode_token[i].name);
	}
	xr_free						(vid_mode_token);
	vid_mode_token				= NULL;
}

void fill_vid_mode_list(CHW* _hw)
{
	if(vid_mode_token != NULL)		return;
	xr_vector<LPCSTR>	_tmp;
	xr_vector<DXGI_MODE_DESC>	modes;

	IDXGIOutput *pOutput;
	_hw->m_pAdapter->EnumOutputs(0, &pOutput);
	VERIFY(pOutput);

	UINT num = 0;
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
	UINT flags         = 0;

	// Get the number of display modes available
	pOutput->GetDisplayModeList( format, flags, &num, 0);

	// Get the list of display modes
	modes.resize(num);
	pOutput->GetDisplayModeList( format, flags, &num, &modes.front());

	_RELEASE(pOutput);

	for (u32 i=0; i<num; ++i)
	{
		DXGI_MODE_DESC &desc = modes[i];
		string32		str;

		if(desc.Width < 800)
			continue;

		xr_sprintf(str, sizeof(str), "%dx%d", desc.Width, desc.Height);

		if(_tmp.end() != std::find_if(_tmp.begin(), _tmp.end(), _uniq_mode(str)))
			continue;

		_tmp.push_back				(NULL);
		_tmp.back()					= xr_strdup(str);
	}
	
	u32 _cnt						= _tmp.size()+1;

	vid_mode_token					= xr_alloc<xr_token>(_cnt);

	vid_mode_token[_cnt-1].id			= -1;
	vid_mode_token[_cnt-1].name		= NULL;

#ifdef DEBUG
	Msg("Available video modes[%d]:",_tmp.size());
#endif // DEBUG
	for( u32 i=0; i<_tmp.size(); ++i )
	{
		vid_mode_token[i].id		= i;
		vid_mode_token[i].name		= _tmp[i];
#ifdef DEBUG
		Msg							("[%s]",_tmp[i]);
#endif // DEBUG
	}
}

void CHW::UpdateViews()
{
	DXGI_SWAP_CHAIN_DESC &sd = m_ChainDesc;
	HRESULT R;

	// Create a render target view
	ID3DTexture2D *pBuffer;
	R = m_pSwapChain->GetBuffer( 0, __uuidof( ID3DTexture2D ), (LPVOID*)&pBuffer );
	R_CHK(R);

	R = pDevice->CreateRenderTargetView( pBuffer, NULL, &pBaseRT);
	pBuffer->Release();
	R_CHK(R);

	//	Create Depth/stencil buffer
	//	HACK: DX10: hard depth buffer format
	ID3DTexture2D* pDepthStencil = NULL;
	D3D_TEXTURE2D_DESC descDepth;
	descDepth.Width = sd.BufferDesc.Width;
	descDepth.Height = sd.BufferDesc.Height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D_USAGE_DEFAULT;
	descDepth.BindFlags = D3D_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	R = pDevice->CreateTexture2D( &descDepth,       // Texture desc
		NULL,                  // Initial data
		&pDepthStencil ); // [out] Texture
	R_CHK(R);

	//	Create Depth/stencil view
	R = pDevice->CreateDepthStencilView( pDepthStencil, NULL, &pBaseZB );
	R_CHK(R);

	pDepthStencil->Release();
}
