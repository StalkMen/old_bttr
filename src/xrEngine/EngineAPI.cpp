// EngineAPI.cpp: implementation of the CEngineAPI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EngineAPI.h"
#include "xr_ioconsole.h"
#include "xr_ioc_cmd.h"
#include "../build_engine_config.h"

void __cdecl dummy(void) 
{
};

CEngineAPI::CEngineAPI()
{
	hGame = 0;
	hRender = 0;
	hTuner = 0;
	pCreate = 0;
	pDestroy = 0;
	tune_pause = dummy;
	tune_resume = dummy;
}

CEngineAPI::~CEngineAPI()
{
	// destroy quality token here
	if (vid_quality_token)
	{
		for (int i = 0; vid_quality_token[i].name; i++)
		{
			xr_free(vid_quality_token[i].name);
		}
		xr_free(vid_quality_token);
		vid_quality_token = nullptr;
	}
}

ENGINE_API bool is_enough_address_space_available()
{
	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);

	return (*(u32*)&system_info.lpMaximumApplicationAddress) > 0x90000000;
}

void CEngineAPI::Initialize()
{
	CCC_LoadCFG_custom pTmp("renderer ");
	pTmp.Execute(Console->ConfigFile);

	constexpr LPCSTR r3_name = "xrRender_R3.dll";
	constexpr LPCSTR r4_name = "xrRender_R4.dll";

	if (psDeviceFlags.test(rDX11))
	{
		Msg("# Loading DLL: [%s]", r4_name);
		hRender = LoadLibrary(r4_name);
		if (!hRender)
			Msg("- [%s] Can't load module: [%s]! Error: %s", __FUNCTION__, r4_name, Debug.error2string(GetLastError()));
	}

	if (psDeviceFlags.test(rDX10))
	{
		Msg("# Loading DLL: [%s]", r3_name);
		hRender = LoadLibrary(r3_name);
		if (!hRender)
			Msg("- [%s] Can't load module: [%s]! Error: %s", __FUNCTION__, r3_name, Debug.error2string(GetLastError()));
	}

	if (!hRender)
	{
		psDeviceFlags.set(rDX11, FALSE);
		psDeviceFlags.set(rDX10, FALSE);
		renderer_value = 0;

		Msg("# Loading DLL: [%s]", r3_name);
		hRender = LoadLibrary(r3_name);
		CRASH_PROTECTION_OGSR(hRender, "- [%s] Can't load module: [%s]! Error: %s", __FUNCTION__, r3_name, Debug.error2string(GetLastError()));
	}

	Device.ConnectToRender();

	constexpr const char* g_name = "xrGame.dll";
	Msg("# Loading DLL: [%s]", g_name);
	hGame = LoadLibrary(g_name);
	CRASH_PROTECTION_OGSR(hGame, "# Game DLL raised exception during loading or there is no game DLL at all. Error: [%s]", Debug.error2string(GetLastError()));
	pCreate = (Factory_Create*)GetProcAddress(hGame, "xrFactory_Create");
	R_ASSERT(pCreate);
	pDestroy = (Factory_Destroy*)GetProcAddress(hGame, "xrFactory_Destroy");
	R_ASSERT(pDestroy);

}

void CEngineAPI::Destroy()
{
	if (hGame) { FreeLibrary(hGame);	hGame = 0; }
	if (hRender) { FreeLibrary(hRender); hRender = 0; }
	pCreate = 0;
	pDestroy = 0;
	Engine.Event._destroy();
	XRC.r_clear_compact();
}

void CEngineAPI::CreateRendererList()
{
	std::vector<std::string> RendererTokens;

	for (size_t i = 3; i <= 4; i++)
	{
		std::string ModuleName("xrRender_R");
		ModuleName += std::to_string(i) + ".dll";

		ModuleHandler RenderModule(ModuleName);
		if (!RenderModule) 
		{
			Msg("- [%s] Can't load module: [%s]! Error: %s", __FUNCTION__, ModuleName.c_str(), Debug.error2string(GetLastError()));
			break;
		}

		if (i == 3)
		{
			//-' Статическое освещение DX10
			//RendererTokens.emplace_back(BttR_mode ? "renderer_dx10_static" : "renderer_r3_static"); // ТОКЕН РЕНДЕРА БУДЕТ НАЧИНАТЬСЯ С 0
			#pragma todo("OldSerpskiStalker. Доделать статический режим рендеров")

			auto dx10_rendering = (SupportsDX10Rendering*)RenderModule.GetProcAddress("SupportsDX10Rendering");
			R_ASSERT(dx10_rendering);
			if (dx10_rendering())
				RendererTokens.emplace_back(BttR_mode ? "renderer_dx10" : "renderer_r3"); // БУДЕТ РАВЕН 1
			else 
			{
				Msg("# [%s] [SupportsDX10Rendering] failed!", __FUNCTION__);
				break;
			}
		}
		else if (i == 4)
		{
			//-' Статическое освещение DX11
			//RendererTokens.emplace_back(BttR_mode ? "renderer_dx11_static" : "renderer_r4_static"); // ТОКЕН БУДЕТ РАВЕН 2
			#pragma todo("OldSerpskiStalker. Доделать статический режим рендеров")

			auto dx11_rendering = (SupportsDX11Rendering*)RenderModule.GetProcAddress("SupportsDX11Rendering");
			R_ASSERT(dx11_rendering);
			if (dx11_rendering())
				RendererTokens.emplace_back(BttR_mode ? "renderer_dx11" : "renderer_r4"); // ТОКЕН БУДЕТ РАВЕН 3
			else 
			{
				Msg("# [%s] [SupportsDX11Rendering] failed!", __FUNCTION__);
				break;
			}
		}
	}

	size_t cnt = RendererTokens.size() + 1;
	vid_quality_token = xr_alloc<xr_token>(cnt);

	vid_quality_token[cnt - 1].id = -1;
	vid_quality_token[cnt - 1].name = nullptr;

	Msg("# [%s] Available render modes [%u]:", __FUNCTION__, RendererTokens.size());
	for (size_t i = 0; i < RendererTokens.size(); ++i)
	{
		vid_quality_token[i].id = i;
		vid_quality_token[i].name = xr_strdup(RendererTokens[i].c_str());
		Msg("# [%s]", RendererTokens[i].c_str());
	}
}