// xrRender_R2.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "../xrRender/dxRenderFactory.h"
#include "../xrRender/dxUIRender.h"
#include "../xrRender/dxDebugRender.h"

#pragma comment(lib,"xrEngine.lib")
#pragma comment(lib,"xrDiscord.lib")

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH	:
		//	Can't call CreateDXGIFactory from DllMain
		//if (!xrRender_test_hw())	return FALSE;
		EnvCryRay.Render					= &RImplementation;
		EnvCryRay.RenderFactory				= &RenderFactoryImpl;
		EnvCryRay.DU						= &DUImpl;
		//::vid_mode_token			= inited by HW;
		EnvCryRay.UIRender					= &UIRenderImpl;
		EnvCryRay.DRender						= &DebugRenderImpl;
		xrRender_initconsole		();
		break	;
	case DLL_THREAD_ATTACH	:
	case DLL_THREAD_DETACH	:
	case DLL_PROCESS_DETACH	:
		break;
	}
	return TRUE;
}


extern "C"
{
#ifdef DIRECTX10
	bool _declspec(dllexport) SupportsDX10Rendering();
#else
	bool _declspec(dllexport) SupportsDX11Rendering();
#endif
};

#ifdef DIRECTX10
bool _declspec(dllexport) SupportsDX10Rendering()
#else
bool _declspec(dllexport) SupportsDX11Rendering()
#endif
{
	return xrRender_test_hw()?true:false;
}