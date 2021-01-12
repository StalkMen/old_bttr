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
		int used_dx10_1; //API, используем ли мы в данный момент DX10.1
		bool detail_settings; //API, разрешить ли регулирование травы через консоль
		bool render_ioconsole; //API, разрешить ли в игре показать консоль
		BOOL static_render; //API, статический режим рендера. 10 или 11
	};
}
extern XRAPI_API CRYRAY_ENV::CRYRAY_ENVIRONMENT	EnvCryRay;