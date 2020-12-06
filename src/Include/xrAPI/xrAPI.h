#pragma once

#ifdef XRAPI_EXPORTS
#define XRAPI_API __declspec(dllexport)
#else
#define XRAPI_API __declspec(dllimport)
#endif

class IRender_interface;
class IRenderFactory;
class CDUInterface;
struct xr_token;
class IUIRender;
class CGameMtlLibrary;
class IDebugRender;

namespace CRYRAY_ENV
{
	class XRAPI_API CRYRAY_ENVIRONMENT
	{
	public:
		IRender_interface* Render;
		IRenderFactory* RenderFactory;
		CDUInterface* DU;
		xr_token* vid_mode_token;
		IUIRender* UIRender;
		CGameMtlLibrary* PGMLib;
		IDebugRender* DRender;

		int used_dx10_1;
	};
}
extern XRAPI_API CRYRAY_ENV::CRYRAY_ENVIRONMENT	EnvCryRay;