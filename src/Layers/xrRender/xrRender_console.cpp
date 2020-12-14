#include	"stdafx.h"
#pragma		hdrstop

#include	"xrRender_console.h"
#include	"dxRenderDeviceRender.h"

#include "../../build_engine_config.h"

extern ENGINE_API u32 renderer_value;

u32			ps_Preset				=	2	;
xr_token							qpreset_token							[ ]={
	{ "Minimum",					0											},
	{ "Low",						1											},
	{ "Default",					2											},
	{ "High",						3											},
	{ "Extreme",					4											},
	{ nullptr,						0											}
};

u32			ps_r3_minmax_sm			=	3;			//	=	0;
xr_token							qminmax_sm_token					[ ]={
	{ "off",						0												},
	{ "on",							1												},
	{ "auto",						2												},
	{ "autodetect",					3												},
	{ 0,							0												}
};

// Common
extern int			psSkeletonUpdate;
extern float		r__dtex_range;

int			ps_r__LightSleepFrames		= 10	;

float		ps_r__Detail_l_ambient		= 0.9f	;
float		ps_r__Detail_l_aniso		= 0.25f	;
float		ps_r__Detail_density		= 0.3f	;
float		ps_r__Detail_rainbow_hemi	= 0.75f	;

float		ps_r__Tree_w_rot			= 10.0f	;
float		ps_r__Tree_w_speed			= 1.00f	;
float		ps_r__Tree_w_amp			= 0.005f;
Fvector		ps_r__Tree_Wave				= {.1f, .01f, .11f};
float		ps_r__Tree_SBC				= 1.5f	;	// scale bias correct

float		ps_r__WallmarkTTL			= 50.f	;
float		ps_r__WallmarkSHIFT			= 0.0001f;
float		ps_r__WallmarkSHIFT_V		= 0.0001f;

int			ps_r__tf_Anisotropic		= 8		;
float		ps_r1_pps_u					= 0.f	;
float		ps_r1_pps_v					= 0.f	;

// R2
float		ps_r2_ssaLOD_A				= 64.f	;
float		ps_r2_ssaLOD_B				= 48.f	;

// R2-specific
Flags32		ps_r2_ls_flags				= 
{ 
	  R2FLAG_SUN 
	| R2FLAG_EXP_DONT_TEST_UNSHADOWED 
	| R2FLAG_USE_NVSTENCIL 
	| R2FLAG_EXP_SPLIT_SCENE 
	| R2FLAG_EXP_MT_CALC 
	| R3FLAG_DYN_WET_SURF
	| R3FLAG_VOLUMETRIC_SMOKE
	| R2FLAG_DETAIL_BUMP
	| R2FLAG_DOF
	| R2FLAG_SOFT_PARTICLES
	| R2FLAG_SOFT_WATER
	| R2FLAG_STEEP_PARALLAX
	| R2FLAG_SUN_FOCUS
	| R2FLAG_SUN_TSM
	| R2FLAG_TONEMAP
	| R2FLAG_VOLUMETRIC_LIGHTS
};	// r2-only

Flags32		ps_r2_ls_flags_ext			= { R2FLAGEXT_ENABLE_TESSELLATION | R2FLAGEXT_SUN_FLARES | R2FLAGEXT_DOF_WEATHER | R2FLAGEXT_RAIN_DROPS_ONLY_HELMET };

BOOL		ps_clear_models_on_unload	= 0; //Alundaio
BOOL		ps_use_precompiled_shaders = 0; //Alundaio
BOOL 		ps_grass_shadow = 0; 			//Alundaio
BOOL		ps_no_scale_on_fade = 0; //Alundaio
float		ps_r2_df_parallax_h			= 0.02f;
float		ps_r2_df_parallax_range		= 75.f;
float		ps_r2_tonemap_middlegray	= 1.f;			// r2-only
float		ps_r2_tonemap_adaptation	= 1.f;				// r2-only
float		ps_r2_tonemap_low_lum		= 0.0001f;			// r2-only
float		ps_r2_tonemap_amount		= 0.7f;				// r2-only
float		ps_r2_ls_bloom_kernel_g		= 3.f;				// r2-only
float		ps_r2_ls_bloom_kernel_b		= .7f;				// r2-only
float		ps_r2_ls_bloom_speed		= 100.f;				// r2-only
float		ps_r2_ls_bloom_kernel_scale	= .7f;				// r2-only	// gauss
float		ps_r2_ls_dsm_kernel			= .7f;				// r2-only
float		ps_r2_ls_psm_kernel			= .7f;				// r2-only
float		ps_r2_ls_ssm_kernel			= .7f;				// r2-only
float		ps_r2_ls_bloom_threshold	= .00001f;				// r2-only
float		ps_r2_mblur					= .2f;				// .5f
int			ps_r2_GI_depth				= 1;				// 1..5
int			ps_r2_GI_photons			= 16;				// 8..64
float		ps_r2_GI_clip				= EPS_L;			// EPS
float		ps_r2_GI_refl				= .9f;				// .9f
float		ps_r2_ls_depth_scale		= 1.00001f;			// 1.00001f
float		ps_r2_ls_depth_bias			= -0.0003f;			// -0.0001f
float		ps_r2_ls_squality			= 1.0f;				// 1.00f
float		ps_r2_sun_tsm_projection	= 0.3f;			// 0.18f
float		ps_r2_sun_tsm_bias			= -0.01f;			// 
float		ps_r2_sun_near				= 20.f;				// 12.0f

extern float OLES_SUN_LIMIT_27_01_07;	//	actually sun_far

float		ps_r2_sun_near_border		= 0.75f;			// 1.0f
float		ps_r2_sun_depth_far_scale	= 1.00000f;			// 1.00001f
float		ps_r2_sun_depth_far_bias	= -0.00002f;			// -0.0000f
float		ps_r2_sun_depth_near_scale	= 1.0000f;			// 1.00001f
float		ps_r2_sun_depth_near_bias	= 0.00001f;		// -0.00005f
//float		ps_r2_sun_lumscale			= 1.0f;				// 1.0f
float		ps_r2_sun_lumscale_hemi		= 1.0f;				// 1.0f
float		ps_r2_sun_lumscale_amb		= 1.0f;
float		ps_r2_gmaterial				= 2.2f;				// 
float		ps_r2_zfill					= 0.25f;				// .1f

float		ps_r2_dhemi_sky_scale		= 0.08f;				// 1.5f
float		ps_r2_dhemi_light_scale     = 0.2f	;
float		ps_r2_dhemi_light_flow      = 0.1f	;
int			ps_r2_dhemi_count			= 5;				// 5
int			ps_r2_wait_sleep			= 0;

float		ps_r2_lt_smooth				= 1.f;				// 1.f
float		ps_r2_slight_fade			= 0.5f;				// 1.f

//	x - min (0), y - focus (1.4), z - max (100)
Fvector3	ps_r2_dof					= Fvector3().set(-1.25f, 1.4f, 600.f);
float		ps_r2_dof_sky				= 30;				//	distance to sky
float		ps_r2_dof_kernel_size		= 5.0f;						//	7.0f

float		ps_r3_dyn_wet_surf_near		= 10.f;				// 10.0f
float		ps_r3_dyn_wet_surf_far		= 30.f;				// 30.0f
int			ps_r3_dyn_wet_surf_sm_res	= 256;				// 256

Flags32 ps_actor_shadow_flags = {0}; //Swartz: actor shadow

//AVO: detail draw radius
Flags32		ps_common_flags = {0};		// r1-only
u32			ps_steep_parallax = 0;
int			ps_r__detail_radius = 100;
#ifdef DETAIL_RADIUS // ���������� �������� ��������� �����
u32			dm_size = 24;
u32 		dm_cache1_line = 12;	//dm_size*2/dm_cache1_count
u32			dm_cache_line = 49;	//dm_size+1+dm_size
u32			dm_cache_size = 2401;	//dm_cache_line*dm_cache_line
float		dm_fade = 47.5;	//float(2*dm_size)-.5f;
u32			dm_current_size = 24;
u32 		dm_current_cache1_line = 12;	//dm_current_size*2/dm_cache1_count
u32			dm_current_cache_line = 49;	//dm_current_size+1+dm_current_size
u32			dm_current_cache_size = 2401;	//dm_current_cache_line*dm_current_cache_line
float		dm_current_fade = 47.5;	//float(2*dm_current_size)-.5f;
#endif
float		ps_current_detail_density = 0.4;
float		ps_current_detail_scale = 0.75f;
xr_token							ext_quality_token[] = {
    {"qt_off", 0},
    {"qt_low", 1},
    {"qt_medium", 2},
    {"qt_high", 3},
    {"qt_extreme", 4},
    {0, 0}
};
//-AVO

//- Mad Max
float		ps_r2_gloss_factor			= 4.0f;
//- Mad Max
#include	"../../xrEngine/xr_ioconsole.h"
#include	"../../xrEngine/xr_ioc_cmd.h"
#include "../xrRenderDX10/StateManager/dx10SamplerStateCache.h"

//-----------------------------------------------------------------------
//AVO: detail draw radius
#ifdef DETAIL_RADIUS
class CCC_detail_radius : public CCC_Integer
{
public:
    void	apply()
    {
        dm_current_size = iFloor((float) ps_r__detail_radius / 4) * 2;
        dm_current_cache1_line = dm_current_size * 2 / 4;		// assuming cache1_count = 4
        dm_current_cache_line = dm_current_size + 1 + dm_current_size;
        dm_current_cache_size = dm_current_cache_line*dm_current_cache_line;
        dm_current_fade = float(2 * dm_current_size) - .5f;
    }
    CCC_detail_radius(LPCSTR N, int* V, int _min = 0, int _max = 999) : CCC_Integer(N, V, _min, _max)
    {};
    virtual void Execute(LPCSTR args)
    {
        CCC_Integer::Execute(args);
        apply();
    }
    virtual void	Status(TStatus& S)
    {
        CCC_Integer::Status(S);
    }
};
//-AVO
#endif

class CCC_tf_Aniso : public CCC_Integer
{
public:
	void	apply() {
		if (0 == DEVICE_HW::CRYRAY_RENDER::HW.pRenderDevice)	return;
		int	val = *value;	clamp(val, 1, 16);

		SSManager.SetMaxAnisotropy(val);

	}
	CCC_tf_Aniso(LPCSTR N, int* v) : CCC_Integer(N, v, 1, 16) { };
	virtual void Execute(LPCSTR args)
	{
		CCC_Integer::Execute(args);
		apply();
	}
	virtual void	Status(TStatus& S)
	{
		CCC_Integer::Status(S);
		apply();
	}
};

class CCC_MipT : public CCC_Float
{
public:
	void	apply()
	{
		if (0 == DEVICE_HW::CRYRAY_RENDER::HW.pRenderDevice)
			return;
		SSManager.SetMipLODBias(*value);
	}

	CCC_MipT(LPCSTR N, float* v) : CCC_Float(N, v, -3.f, +3.f) { };
	virtual void Execute(LPCSTR args)
	{
		CCC_Float::Execute(args);
		apply();
	}
	virtual void	Status(TStatus& S)
	{
		CCC_Float::Status(S);
		apply();
	}
};

class CCC_R2GM		: public CCC_Float
{
public:
	CCC_R2GM(LPCSTR N, float*	v) : CCC_Float(N, v, 0.f, 4.f) { *v = 0; };
	virtual void	Execute	(LPCSTR args)
	{
		if (0==xr_strcmp(args,"on"))	{
			ps_r2_ls_flags.set	(R2FLAG_GLOBALMATERIAL,TRUE);
		} else if (0==xr_strcmp(args,"off"))	{
			ps_r2_ls_flags.set	(R2FLAG_GLOBALMATERIAL,FALSE);
		} else {
			CCC_Float::Execute	(args);
			if (ps_r2_ls_flags.test(R2FLAG_GLOBALMATERIAL))	{
				static LPCSTR	name[4]	=	{ "oren", "blin", "phong", "metal" };
				float	mid		= *value	;
				int		m0		= iFloor(mid)	% 4;
				int		m1		= (m0+1)		% 4;
				float	frc		= mid - float(iFloor(mid));
				Msg		("* material set to [%s]-[%s], with lerp of [%f]",name[m0],name[m1],frc);
			}
		}
	}
};
class CCC_Screenshot : public IConsole_Command
{
public:
	CCC_Screenshot(LPCSTR N) : IConsole_Command(N)  { };
	virtual void Execute(LPCSTR args) {
		if (g_dedicated_server)
			return;

		string_path	name;	name[0]=0;
		sscanf		(args,"%s",	name);
		LPCSTR		image	= xr_strlen(name)?name:0;
		EnvCryRay.Render->Screenshot(IRender_interface::SM_NORMAL,image);
	}
};

class CCC_RestoreQuadIBData : public IConsole_Command
{
public:
	CCC_RestoreQuadIBData(LPCSTR N) : IConsole_Command(N)  { };
	virtual void Execute(LPCSTR args) {
		RCache.RestoreQuadIBData();
	}
};

class CCC_ModelPoolStat : public IConsole_Command
{
public:
	CCC_ModelPoolStat(LPCSTR N) : IConsole_Command(N)  { bEmptyArgsHandled = TRUE; };
	virtual void Execute(LPCSTR args) {
		RImplementation.Models->dump();
	}
};

//-----------------------------------------------------------------------
class	CCC_Preset		: public CCC_Token
{
public:
	CCC_Preset(LPCSTR N, u32* V, xr_token* T) : CCC_Token(N,V,T)	{}	;

	virtual void	Execute	(LPCSTR args)	{
		CCC_Token::Execute	(args);
		string_path		_cfg;
		string_path		cmd;
		
		switch	(*value)	
		{
			case 0:		
				if (Call_of_Chernobyl_mode)
					xr_strcpy(_cfg, "rspec_minimum.ltx");	
				else
					xr_strcpy(_cfg, "xrRender_DX_settings\\minimum_dx.ltx");
				break;
			case 1:		
				if (Call_of_Chernobyl_mode)
					xr_strcpy(_cfg, "rspec_low.ltx");		
				else
					xr_strcpy(_cfg, "xrRender_DX_settings\\low_dx.ltx");
				break;
			case 2:		
				if (Call_of_Chernobyl_mode)
					xr_strcpy(_cfg, "rspec_default.ltx");	
				else
					xr_strcpy(_cfg, "xrRender_DX_settings\\default_dx.ltx");
				break;
			case 3:		
				if (Call_of_Chernobyl_mode)
					xr_strcpy(_cfg, "rspec_high.ltx");	
				else
					xr_strcpy(_cfg, "xrRender_DX_settings\\high_dx.ltx");
				break;
			case 4:	
				if (Call_of_Chernobyl_mode)
					xr_strcpy(_cfg, "rspec_extreme.ltx");	
				else
					xr_strcpy(_cfg, "xrRender_DX_settings\\extreme_dx.ltx");
				break;
		}
		FS.update_path			(_cfg,"$game_config$",_cfg);
		strconcat				(sizeof(cmd),cmd,"cfg_load", " ", _cfg);
		Console->Execute		(cmd);
	}
};


class CCC_memory_stats : public IConsole_Command
{
protected	:

public		:

	CCC_memory_stats(LPCSTR N) :	IConsole_Command(N)	{ bEmptyArgsHandled = true; };

	virtual void	Execute	(LPCSTR args)
	{
		u32 m_base = 0;
		u32 c_base = 0;
		u32 m_lmaps = 0; 
		u32 c_lmaps = 0;

		dxRenderDeviceRender::Instance().ResourcesGetMemoryUsage( m_base, c_base, m_lmaps, c_lmaps );

		Msg		("memory usage  mb \t \t video    \t managed      \t system \n" );

		float vb_video		= (float)DEVICE_HW::CRYRAY_RENDER::HW.stats_manager.memory_usage_summary[enum_stats_buffer_type_vertex][D3DPOOL_DEFAULT]/1024/1024;
		float vb_managed	= (float)DEVICE_HW::CRYRAY_RENDER::HW.stats_manager.memory_usage_summary[enum_stats_buffer_type_vertex][D3DPOOL_MANAGED]/1024/1024;
		float vb_system		= (float)DEVICE_HW::CRYRAY_RENDER::HW.stats_manager.memory_usage_summary[enum_stats_buffer_type_vertex][D3DPOOL_SYSTEMMEM]/1024/1024;
		Msg		("vertex buffer      \t \t %f \t %f \t %f ",	vb_video, vb_managed, vb_system);

		float ib_video		= (float)DEVICE_HW::CRYRAY_RENDER::HW.stats_manager.memory_usage_summary[enum_stats_buffer_type_index][D3DPOOL_DEFAULT]/1024/1024; 
		float ib_managed	= (float)DEVICE_HW::CRYRAY_RENDER::HW.stats_manager.memory_usage_summary[enum_stats_buffer_type_index][D3DPOOL_MANAGED]/1024/1024; 
		float ib_system		= (float)DEVICE_HW::CRYRAY_RENDER::HW.stats_manager.memory_usage_summary[enum_stats_buffer_type_index][D3DPOOL_SYSTEMMEM]/1024/1024; 
		Msg		("index buffer      \t \t %f \t %f \t %f ",	ib_video, ib_managed, ib_system);
		
		float textures_managed = (float)(m_base+m_lmaps)/1024/1024;
		Msg		("textures          \t \t %f \t %f \t %f ",	0.f, textures_managed, 0.f);

		float rt_video		= (float)DEVICE_HW::CRYRAY_RENDER::HW.stats_manager.memory_usage_summary[enum_stats_buffer_type_rtarget][D3DPOOL_DEFAULT]/1024/1024;
		float rt_managed	= (float)DEVICE_HW::CRYRAY_RENDER::HW.stats_manager.memory_usage_summary[enum_stats_buffer_type_rtarget][D3DPOOL_MANAGED]/1024/1024;
		float rt_system		= (float)DEVICE_HW::CRYRAY_RENDER::HW.stats_manager.memory_usage_summary[enum_stats_buffer_type_rtarget][D3DPOOL_SYSTEMMEM]/1024/1024;
		Msg		("R-Targets         \t \t %f \t %f \t %f ",	rt_video, rt_managed, rt_system);									

		Msg		("\nTotal             \t \t %f \t %f \t %f ",	vb_video+ib_video+rt_video,
																textures_managed + vb_managed+ib_managed+rt_managed,
																vb_system+ib_system+rt_system);
	}

};

#include "r__pixel_calculator.h"
class CCC_BuildSSA : public IConsole_Command
{
public:
	CCC_BuildSSA(LPCSTR N) : IConsole_Command(N)  { bEmptyArgsHandled = TRUE; };
	virtual void Execute(LPCSTR args) 
	{
	}
};

class CCC_DofFar : public CCC_Float
{
public:
	CCC_DofFar(LPCSTR N, float* V, float _min=0.0f, float _max=10000.0f) 
		: CCC_Float( N, V, _min, _max){}

	virtual void Execute(LPCSTR args) 
	{
		float v = float(atof(args));

		if (v<ps_r2_dof.y+0.1f)
		{
			char	pBuf[256];
			_snprintf( pBuf, sizeof(pBuf)/sizeof(pBuf[0]), "float value greater or equal to r2_dof_focus+0.1");
			Msg("~ Invalid syntax in call to '%s'",cName);
			Msg("~ Valid arguments: %s", pBuf);
			Console->Execute("r2_dof_focus");
		}
		else
		{
			CCC_Float::Execute(args);
			if(g_pGamePersistent)
				g_pGamePersistent->SetBaseDof(ps_r2_dof);
		}
	}

	//	CCC_Dof should save all data as well as load from config
	virtual void	Save	(IWriter *F)	{;}
};

class CCC_DofNear : public CCC_Float
{
public:
	CCC_DofNear(LPCSTR N, float* V, float _min=0.0f, float _max=10000.0f) 
		: CCC_Float( N, V, _min, _max){}

	virtual void Execute(LPCSTR args) 
	{
		float v = float(atof(args));

		if (v>ps_r2_dof.y-0.1f)
		{
			char	pBuf[256];
			_snprintf( pBuf, sizeof(pBuf)/sizeof(pBuf[0]), "float value less or equal to r2_dof_focus-0.1");
			Msg("~ Invalid syntax in call to '%s'",cName);
			Msg("~ Valid arguments: %s", pBuf);
			Console->Execute("r2_dof_focus");
		}
		else
		{
			CCC_Float::Execute(args);
			if(g_pGamePersistent)
				g_pGamePersistent->SetBaseDof(ps_r2_dof);
		}
	}

	//	CCC_Dof should save all data as well as load from config
	virtual void	Save	(IWriter *F)	{;}
};

class CCC_DofFocus : public CCC_Float
{
public:
	CCC_DofFocus(LPCSTR N, float* V, float _min=0.0f, float _max=10000.0f) 
		: CCC_Float( N, V, _min, _max){}

	virtual void Execute(LPCSTR args) 
	{
		float v = float(atof(args));

		if (v>ps_r2_dof.z-0.1f)
		{
			char	pBuf[256];
			_snprintf( pBuf, sizeof(pBuf)/sizeof(pBuf[0]), "float value less or equal to r2_dof_far-0.1");
			Msg("~ Invalid syntax in call to '%s'",cName);
			Msg("~ Valid arguments: %s", pBuf);
			Console->Execute("r2_dof_far");
		}
		else if (v<ps_r2_dof.x+0.1f)
		{
			char	pBuf[256];
			_snprintf( pBuf, sizeof(pBuf)/sizeof(pBuf[0]), "float value greater or equal to r2_dof_far-0.1");
			Msg("~ Invalid syntax in call to '%s'",cName);
			Msg("~ Valid arguments: %s", pBuf);
			Console->Execute("r2_dof_near");
		}
		else{
			CCC_Float::Execute(args);
			if(g_pGamePersistent)
				g_pGamePersistent->SetBaseDof(ps_r2_dof);
			}
	}

	//	CCC_Dof should save all data as well as load from config
	virtual void	Save	(IWriter *F)	{;}
};

class CCC_Dof : public CCC_Vector3
{
public:
	CCC_Dof(LPCSTR N, Fvector* V, const Fvector _min, const Fvector _max) : 
	  CCC_Vector3(N, V, _min, _max) {;}

	virtual void	Execute	(LPCSTR args)
	{
		Fvector v;
		if (3!=sscanf(args,"%f,%f,%f",&v.x,&v.y,&v.z))	
			InvalidSyntax(); 
		else if ( (v.x > v.y-0.1f) || (v.z < v.y+0.1f))
		{
			InvalidSyntax();
			Msg("x <= y - 0.1");
			Msg("y <= z - 0.1");
		}
		else
		{
			CCC_Vector3::Execute(args);
			if(g_pGamePersistent)
				g_pGamePersistent->SetBaseDof(ps_r2_dof);
		}
	}
	virtual void	Status	(TStatus& S)
	{	
		xr_sprintf	(S,"%f,%f,%f",value->x,value->y,value->z);
	}
	virtual void	Info	(TInfo& I)
	{	
		xr_sprintf(I,"vector3 in range [%f,%f,%f]-[%f,%f,%f]",min.x,min.y,min.z,max.x,max.y,max.z);
	}

};

class CCC_DumpResources : public IConsole_Command
{
public:
	CCC_DumpResources(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = TRUE; };
	virtual void Execute(LPCSTR args) {
		RImplementation.Models->dump();
		dxRenderDeviceRender::Instance().Resources->Dump(false);
	}
};

//	Allow real-time fog config reload

#ifdef	DEBUG

#include "../xrRenderDX10/3DFluid/dx103DFluidManager.h"

class CCC_Fog_Reload : public IConsole_Command
{
public:
	CCC_Fog_Reload(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = TRUE; };
	virtual void Execute(LPCSTR args) 
	{
		FluidManager.UpdateProfiles();
	}
};
#endif	//	DEBUG

//OldSerpskiStalker, технические флаги
Flags32 ps_r__common_flags = { RFLAGDX_ENABLE_DEBUG_LOG | RFLAGDX_OLD_LOAD_DDS };

u32 ps_r2_smapsize = 2048;
xr_token qsmapsize_token[] =
{
	{ "st_size_1536", 1536 },
	{ "st_size_2048", 2048 },
	{ "st_size_2560", 2560 },
	{ "st_size_3072", 3072 },
	{ "st_size_3584", 3584 },
	{ "st_size_4096", 4096 },
	{ "st_size_8192", 8192 },
	{ nullptr, 0 }
};

u32	ps_r_type_screen = 1;
xr_token screen_mode_token[] = {
{ "jpg",	  1},
{ "png",	  2},
{ "bmp",	  3},
{ nullptr,	  0}
};

float ps_r__tf_Mipbias = 0.0f;
float ps_r2_ss_sunshafts_length	= 1.f;
float ps_r2_ss_sunshafts_radius	= 1.f;

float debug_fog_height = 0.f;
float debug_fog_density = 0.f;
float debug_fog_max_dist = 0.f;
float debug_fog_min_dist = 0.f;

float ps_volumetric_intensity = 0.1f;
float ps_volumetric_distance = 1.f;
float ps_volumetric_quality = 0.35f;

float xrRenderFilteringSaturationImage = 0.f;
float render_gamma = 0.f;

// Base factor values
float ps_r__GLOD_ssa_start		= 256.f;
float ps_r__GLOD_ssa_end		= 64.f;
float ps_r__ssaDISCARD			= 3.5f;
float ps_r__ssaHZBvsTEX			= 96.f;

// Distance factor values
float ps_r__geomLodSpriteDistF_ = 2.0f;
float ps_r__geomDiscardDistF_	= 2.0f;
float ps_r__geomLodDistF_		= 2.0f;
float ps_r__geomNTextureDistF_  = 2.0f;
float ps_r__geomDTextureDistF_  = 2.0f;

int   tbufer_renders = 1;
int	  ps_render_volumetric_fog = 1;
int	  render_nightvision = 0;
int   opt_static = 2;
int   opt_dynamic = 2;
BOOL  opt_shadow = 1;

BOOL  rain_drops = 1;
BOOL  mask_helmet = 1;
Fvector ps_r2_drops_control = { 0.0f, 0.0f, 0.0f };
Fvector ps_r2_gasmask_control = { 0.0f, 0.0f, 0.0f };
//-----------------------------------------------------------------------
void		xrRender_initconsole()
{
//////////////////////////////////////////////////////////////////////////////////////////////////
#pragma todo("OldSerpskiStalker. Новые консольные команды")
	CMD2(CCC_MipT,  "xrRenderDX10_mipTextures", 	 	 &ps_r__tf_Mipbias);
	CMD3(CCC_Mask,  "xrRenderDX10_no_ram",				 &ps_r__common_flags,				RFLAG_NO_RAM_TEXTURES);
	CMD3(CCC_Token, "xrRenderDX10_shadow_map_size",		 &ps_r2_smapsize,					qsmapsize_token);
	CMD3(CCC_Mask,  "xrRenderDX10_shader_cache",		 &ps_r__common_flags,				RFLAGDX10_NO_SHADER_CACHE);
#pragma todo("OldSerpskiStalker. Disabled.")
#if 0
	CMD4(CCC_Float,	"xrRenderDX10_ss_sunshafts_length",	 &ps_r2_ss_sunshafts_length, 		.2f, 1.5f);
	CMD4(CCC_Float,	"xrRenderDX10_ss_sunshafts_radius",	 &ps_r2_ss_sunshafts_radius, 		.5f, 2.f);
#endif
	CMD3(CCC_Mask,  "xrRenderDX_enable_debug_log",		 &ps_r__common_flags,				RFLAGDX_ENABLE_DEBUG_LOG);
	CMD3(CCC_Token, "xrRenderDX_type_screenshot",		 &ps_r_type_screen,					screen_mode_token);
#ifdef DIRECTX11
	CMD3(CCC_Mask,  "xrRenderDX11_shader_cache",		 &ps_r__common_flags,				RFLAGDX11_NO_SHADER_CACHE);
#endif
	CMD4(CCC_Integer, "xrRenderDX10_triple_buffering",	 &tbufer_renders,					0, 1);
	CMD4(CCC_Integer, "xrRenderDX10_rain_drops",		 &rain_drops,						0, 1);
	CMD4(CCC_Integer, "xrRenderDX10_mask_helmet",		 &mask_helmet,						0, 1);

	CMD3(CCC_Mask,  "xrRenderDX10_animMBlur_move",		 &ps_r__common_flags,				RFLAGDX10_MOTION_BLUR);

	Fvector	tw_min, tw_max;
	const bool disable_this_command = false;
	if (disable_this_command)
		CMD3(CCC_Mask, "xrRenderDX_old_load_dds",		 &ps_r__common_flags,				RFLAGDX_OLD_LOAD_DDS);

	CMD4(CCC_Float, "fog_height",						 &debug_fog_height,					0.f, 10.f);
	CMD4(CCC_Float, "fog_density",						 &debug_fog_density,				0.f, 1.f);
	CMD4(CCC_Float, "fog_max_dist",						 &debug_fog_max_dist,				0.f, 100.f);
	CMD4(CCC_Float, "fog_min_dist",						 &debug_fog_min_dist,				0.f, 90.f);
	CMD4(CCC_Integer, "xrRenderDX10_volumetric_fog",	 &ps_render_volumetric_fog,			0, 1);

	CMD4(CCC_Float, "xrRenderDX_volum_intensity",		 &ps_volumetric_intensity,			-1.f, 1.f);
	CMD4(CCC_Float, "xrRenderDX_volum_distance",		 &ps_volumetric_distance,			0.f, 2.f);
	CMD4(CCC_Float, "xrRenderDX_volum_quality",			 &ps_volumetric_quality,			0.f, 1.f);

	CMD3(CCC_Mask, "xrRenderDX10_sun_flares",			 &ps_r2_ls_flags_ext,				R2FLAGEXT_SUN_FLARES);
	CMD3(CCC_Mask, "xrRenderDX10_dof_weather",			 &ps_r2_ls_flags_ext,				R2FLAGEXT_DOF_WEATHER);

	CMD3(CCC_Mask, "xrRenderDX10_rain_drops_only_helmet", &ps_r2_ls_flags_ext,				R2FLAGEXT_RAIN_DROPS_ONLY_HELMET);

#ifdef DIRECTX10
	CMD3(CCC_Mask, "xrRenderDX10_use_device1",			&ps_r2_ls_flags,					(u32)R3FLAG_USE_DX10_1);
#endif

	CMD4(CCC_Integer, "xrRenderDX10_opt_static_geometry",	&opt_static,					0, 4);
	CMD4(CCC_Integer, "xrRenderDX10_opt_dynamic_geometry",	&opt_dynamic,					0, 4);
	CMD4(CCC_Integer, "xrRenderDX10_opt_shadow_geometry",	&opt_shadow,					0, 1);

	CMD4(CCC_Float, "xrRenderFilteringSaturationImage",  &xrRenderFilteringSaturationImage,	-1.f, 2.f);

	CMD4(CCC_Float, "xrRenderGammaHLSL",				 &render_gamma,						0.f, 3.f);

	//LVutner 
	CMD4(CCC_Integer, "r_nv",							 &render_nightvision,				0, 3);

	CMD4(CCC_Float, "r__ssa_glod_start",				 &ps_r__GLOD_ssa_start,				128, 512);
	CMD4(CCC_Float, "r__ssa_glod_end",					 &ps_r__GLOD_ssa_end,				16, 96);
	CMD4(CCC_Float, "r__lod_sprite_dist_f",				 &ps_r__geomLodSpriteDistF_,		0.1f, 3.0f);
	CMD4(CCC_Float, "r__geom_quality_dist_f",			 &ps_r__geomLodDistF_,				0.1f, 3.0f);
	CMD4(CCC_Float, "r__geom_discard_dist_f",			 &ps_r__geomDiscardDistF_,			0.1f, 3.0f);
	CMD4(CCC_Float, "r__dtexture_dist_f",				 &ps_r__geomDTextureDistF_,			0.1f, 3.0f);
	CMD4(CCC_Float, "r__ntexture_dist_f",				 &ps_r__geomNTextureDistF_,			0.1f, 3.0f);

	tw_min.set(0, 0, 0);
	tw_max.set(2, 3, 1);
	CMD4(CCC_Vector3, "xrRenderDropsControl",		 &ps_r2_drops_control, tw_min, tw_max);
	
	tw_min.set(0, (float)0.f, 0);
	tw_max.set(0, (float)1.f, 1);
	CMD4(CCC_Vector3, "xrRenderGasmaskControl",		 &ps_r2_gasmask_control, tw_min, tw_max);
//////////////////////////////////////////////////////////////////////////////////////////////////

	CMD3(CCC_Preset, "_preset", &ps_Preset, qpreset_token);

	CMD4(CCC_Integer, "rs_skeleton_update", &psSkeletonUpdate, 2, 128);
#ifdef	DEBUG
	CMD1(CCC_DumpResources, "dump_resources");
#endif	//	 DEBUG

	CMD4(CCC_Float, "r__dtex_range", &r__dtex_range, 5, 175);

	// Common
	CMD1(CCC_Screenshot, "screenshot");

	//	Igor: just to test bug with rain/particles corruption
	CMD1(CCC_RestoreQuadIBData, "r_restore_quad_ib_data");
#ifdef DEBUG
	CMD1(CCC_BuildSSA, "build_ssa");
	CMD4(CCC_Integer, "r__lsleep_frames", &ps_r__LightSleepFrames, 4, 30);
	CMD4(CCC_Float, "r__wallmark_shift_pp", &ps_r__WallmarkSHIFT, 0.0f, 1.f);
	CMD4(CCC_Float, "r__wallmark_shift_v", &ps_r__WallmarkSHIFT_V, 0.0f, 1.f);
	CMD1(CCC_ModelPoolStat, "stat_models");
#endif // DEBUG
	CMD4(CCC_Float, "r__wallmark_ttl", &ps_r__WallmarkTTL, 1.0f, 5.f * 60.f);

#ifdef DEBUG
	CMD4(CCC_Float, "r__detail_l_ambient", &ps_r__Detail_l_ambient, .5f, .95f);
	CMD4(CCC_Float, "r__detail_l_aniso", &ps_r__Detail_l_aniso, .1f, .5f);

	CMD4(CCC_Float, "r__d_tree_w_amp", &ps_r__Tree_w_amp, .001f, 1.f);
	CMD4(CCC_Float, "r__d_tree_w_rot", &ps_r__Tree_w_rot, .01f, 100.f);
	CMD4(CCC_Float, "r__d_tree_w_speed", &ps_r__Tree_w_speed, 1.0f, 10.f);

	tw_min.set(EPS, EPS, EPS);
	tw_max.set(2, 2, 2);
	CMD4(CCC_Vector3, "r__d_tree_wave", &ps_r__Tree_Wave, tw_min, tw_max);
#endif // DEBUG

	CMD2(CCC_tf_Aniso, "r__tf_aniso", &ps_r__tf_Anisotropic); //	{1..16}

	CMD4(CCC_Float, "r1_pps_u", &ps_r1_pps_u, -1.f, +1.f);
	CMD4(CCC_Float, "r1_pps_v", &ps_r1_pps_v, -1.f, +1.f);

	// R2
	CMD4(CCC_Float, "r2_ssa_lod_a", &ps_r2_ssaLOD_A, 16, 96);
	CMD4(CCC_Float, "r2_ssa_lod_b", &ps_r2_ssaLOD_B, 32, 64);

	// R2-specific
	CMD2(CCC_R2GM, "r2em", &ps_r2_gmaterial);
	CMD3(CCC_Mask, "r2_tonemap", &ps_r2_ls_flags, R2FLAG_TONEMAP);
	CMD4(CCC_Float, "r2_tonemap_middlegray", &ps_r2_tonemap_middlegray, 0.0f, 2.0f);
	CMD4(CCC_Float, "r2_tonemap_adaptation", &ps_r2_tonemap_adaptation, 0.01f, 10.0f);
	CMD4(CCC_Float, "r2_tonemap_lowlum", &ps_r2_tonemap_low_lum, 0.0001f, 1.0f);
	CMD4(CCC_Float, "r2_tonemap_amount", &ps_r2_tonemap_amount, 0.0000f, 1.0f);
	CMD4(CCC_Float, "r2_ls_bloom_kernel_scale", &ps_r2_ls_bloom_kernel_scale, 0.05f, 2.f);
	CMD4(CCC_Float, "r2_ls_bloom_kernel_g", &ps_r2_ls_bloom_kernel_g, 1.f, 7.f);
	CMD4(CCC_Float, "r2_ls_bloom_kernel_b", &ps_r2_ls_bloom_kernel_b, 0.01f, 1.f);
	CMD4(CCC_Float, "r2_ls_bloom_threshold", &ps_r2_ls_bloom_threshold, 0.f, 1.f);
	CMD4(CCC_Float, "r2_ls_bloom_speed", &ps_r2_ls_bloom_speed, 0.f, 100.f);
	CMD3(CCC_Mask, "r2_ls_bloom_fast", &ps_r2_ls_flags, R2FLAG_FASTBLOOM);
	CMD4(CCC_Float, "r2_ls_dsm_kernel", &ps_r2_ls_dsm_kernel, .1f, 3.f);
	CMD4(CCC_Float, "r2_ls_psm_kernel", &ps_r2_ls_psm_kernel, .1f, 3.f);
	CMD4(CCC_Float, "r2_ls_ssm_kernel", &ps_r2_ls_ssm_kernel, .1f, 3.f);
	CMD4(CCC_Float, "r2_ls_squality", &ps_r2_ls_squality, .5f, 1.f);

	CMD3(CCC_Mask, "r2_zfill", &ps_r2_ls_flags, R2FLAG_ZFILL);
	CMD4(CCC_Float, "r2_zfill_depth", &ps_r2_zfill, .001f, .5f);
	CMD3(CCC_Mask, "r2_allow_r1_lights", &ps_r2_ls_flags, R2FLAG_R1LIGHTS);

	CMD3(CCC_Mask, "r__actor_shadow", &ps_actor_shadow_flags, RFLAG_ACTOR_SHADOW);  //Swartz: actor shadow

	//- Mad Max
	CMD4(CCC_Float, "r2_gloss_factor", &ps_r2_gloss_factor, .0f, 10.f);
	//- Mad Max

#ifdef DEBUG
	CMD3(CCC_Mask, "r2_use_nvdbt", &ps_r2_ls_flags, R2FLAG_USE_NVDBT);
	CMD3(CCC_Mask, "r2_mt", &ps_r2_ls_flags, R2FLAG_EXP_MT_CALC);
#endif // DEBUG

	CMD3(CCC_Mask, "r2_sun", &ps_r2_ls_flags, R2FLAG_SUN);
	CMD3(CCC_Mask, "r2_sun_details", &ps_r2_ls_flags, R2FLAG_SUN_DETAILS);
	CMD3(CCC_Mask, "r2_sun_focus", &ps_r2_ls_flags, R2FLAG_SUN_FOCUS);
	CMD3(CCC_Mask, "r2_exp_donttest_shad", &ps_r2_ls_flags, R2FLAG_EXP_DONT_TEST_SHADOWED);

	CMD3(CCC_Mask, "r2_sun_tsm", &ps_r2_ls_flags, R2FLAG_SUN_TSM);
	CMD4(CCC_Float, "r2_sun_tsm_proj", &ps_r2_sun_tsm_projection, .001f, 0.8f);
	CMD4(CCC_Float, "r2_sun_tsm_bias", &ps_r2_sun_tsm_bias, -0.5, +0.5);
	CMD4(CCC_Float, "r2_sun_near", &ps_r2_sun_near, 1.f,	/*50.f*/150.f); //AVO: extended from 50 to 150
	CMD4(CCC_Float, "r2_sun_far", &OLES_SUN_LIMIT_27_01_07, 51.f, 180.f);
	CMD4(CCC_Float, "r2_sun_near_border", &ps_r2_sun_near_border, .5f, 1.0f);
	CMD4(CCC_Float, "r2_sun_depth_far_scale", &ps_r2_sun_depth_far_scale, 0.5, 1.5);
	CMD4(CCC_Float, "r2_sun_depth_far_bias", &ps_r2_sun_depth_far_bias, -0.5, +0.5);
	CMD4(CCC_Float, "r2_sun_depth_near_scale", &ps_r2_sun_depth_near_scale, 0.5, 1.5);
	CMD4(CCC_Float, "r2_sun_depth_near_bias", &ps_r2_sun_depth_near_bias, -0.5, +0.5);
//	CMD4(CCC_Float, "r2_sun_lumscale", &ps_r2_sun_lumscale, -1.0, +3.0);
	CMD4(CCC_Float, "r2_sun_lumscale_hemi", &ps_r2_sun_lumscale_hemi, 0.0, +3.0);
	CMD4(CCC_Float, "r2_sun_lumscale_amb", &ps_r2_sun_lumscale_amb, 0.0, +3.0);
	CMD4(CCC_Float, "r2_mblur", &ps_r2_mblur, 0.15f, 1.0f);

	CMD3(CCC_Mask, "r2_gi", &ps_r2_ls_flags, R2FLAG_GI);
	CMD4(CCC_Float, "r2_gi_clip", &ps_r2_GI_clip, EPS, 0.1f);
	CMD4(CCC_Integer, "r2_gi_depth", &ps_r2_GI_depth, 1, 5);
	CMD4(CCC_Integer, "r2_gi_photons", &ps_r2_GI_photons, 8, 256);
	CMD4(CCC_Float, "r2_gi_refl", &ps_r2_GI_refl, EPS_L, 0.99f);

	CMD4(CCC_Integer, "r2_wait_sleep", &ps_r2_wait_sleep, 0, 1);

#ifndef MASTER_GOLD
	CMD4(CCC_Integer, "r2_dhemi_count", &ps_r2_dhemi_count, 4, 25);
	CMD4(CCC_Float, "r2_dhemi_sky_scale", &ps_r2_dhemi_sky_scale, 0.0f, 100.f);
	CMD4(CCC_Float, "r2_dhemi_light_scale", &ps_r2_dhemi_light_scale, 0, 100.f);
	CMD4(CCC_Float, "r2_dhemi_light_flow", &ps_r2_dhemi_light_flow, 0, 1.f);
	CMD4(CCC_Float, "r2_dhemi_smooth", &ps_r2_lt_smooth, 0.f, 10.f);
	CMD3(CCC_Mask, "rs_hom_depth_draw", &ps_r2_ls_flags_ext, R_FLAGEXT_HOM_DEPTH_DRAW);
	CMD3(CCC_Mask, "r2_shadow_cascede_zcul", &ps_r2_ls_flags_ext, R2FLAGEXT_SUN_ZCULLING);
	CMD3(CCC_Mask, "r2_shadow_cascede_old", &ps_r2_ls_flags_ext, R2FLAGEXT_SUN_OLD);
#endif 

	CMD4(CCC_Float, "r2_ls_depth_scale", &ps_r2_ls_depth_scale, 0.5, 1.5);
	CMD4(CCC_Float, "r2_ls_depth_bias", &ps_r2_ls_depth_bias, -0.5, +0.5);

	CMD4(CCC_Float, "r2_parallax_h", &ps_r2_df_parallax_h, .0f, .5f);

	CMD4(CCC_Float, "r2_slight_fade", &ps_r2_slight_fade, .2f, 1.f);

	//	Igor: Depth of field
	tw_min.set(-10000, -10000, 0);	tw_max.set(10000, 10000, 10000);
	CMD4(CCC_Dof, "r2_dof", &ps_r2_dof, tw_min, tw_max);
	CMD4(CCC_DofNear, "r2_dof_near", &ps_r2_dof.x, tw_min.x, tw_max.x);
	CMD4(CCC_DofFocus, "r2_dof_focus", &ps_r2_dof.y, tw_min.y, tw_max.y);
	CMD4(CCC_DofFar, "r2_dof_far", &ps_r2_dof.z, tw_min.z, tw_max.z);

	CMD4(CCC_Float, "r2_dof_kernel", &ps_r2_dof_kernel_size, .0f, 10.f);
	CMD4(CCC_Float, "r2_dof_sky", &ps_r2_dof_sky, -10000.f, 10000.f);
	CMD3(CCC_Mask, "r2_dof_enable", &ps_r2_ls_flags, R2FLAG_DOF);
	
	CMD3(CCC_Mask, "r2_volumetric_lights", &ps_r2_ls_flags, R2FLAG_VOLUMETRIC_LIGHTS);

	CMD3(CCC_Mask, "r4_enable_tessellation", &ps_r2_ls_flags_ext, R2FLAGEXT_ENABLE_TESSELLATION);//Need restart
	CMD3(CCC_Mask, "r4_wireframe", &ps_r2_ls_flags_ext, R2FLAGEXT_WIREFRAME);//Need restart
	CMD3(CCC_Mask, "r2_steep_parallax", &ps_r2_ls_flags, R2FLAG_STEEP_PARALLAX);
	CMD3(CCC_Mask, "r2_detail_bump", &ps_r2_ls_flags, R2FLAG_DETAIL_BUMP);

	//	Igor: need restart
	CMD3(CCC_Mask, "r2_soft_water", &ps_r2_ls_flags, R2FLAG_SOFT_WATER);
	CMD3(CCC_Mask, "r2_soft_particles", &ps_r2_ls_flags, R2FLAG_SOFT_PARTICLES);

	CMD3(CCC_Token, "r3_minmax_sm", &ps_r3_minmax_sm, qminmax_sm_token);

#ifdef DETAIL_RADIUS
	if (EnvCryRay.detail_settings)
	{
		CMD4(CCC_Float, "r__detail_density", &ps_current_detail_density, 0.04f, 0.6f);
		CMD4(CCC_Float, "r__detail_scale", &ps_current_detail_scale, 0.5f, 2.0f);
		CMD4(CCC_detail_radius, "r__detail_radius", &ps_r__detail_radius, 49, 250);
	}

	CMD4(CCC_Integer, "r__clear_models_on_unload", &ps_clear_models_on_unload, 0, 1); //Alundaio
	CMD4(CCC_Integer, "r__use_precompiled_shaders", &ps_use_precompiled_shaders, 0, 1); //Alundaio
	CMD4(CCC_Integer, "r__enable_grass_shadow", &ps_grass_shadow, 0, 1); //Alundaio
	CMD4(CCC_Integer, "r__no_scale_on_fade", &ps_no_scale_on_fade, 0, 1); //Alundaio
#endif

	//	Allow real-time fog config reload

#ifdef	DEBUG
	CMD1(CCC_Fog_Reload, "r3_fog_reload");
#endif	//	DEBUG

	CMD3(CCC_Mask, "r3_dynamic_wet_surfaces", &ps_r2_ls_flags, R3FLAG_DYN_WET_SURF);
	CMD4(CCC_Float, "r3_dynamic_wet_surfaces_near", &ps_r3_dyn_wet_surf_near, 10, 70);
	CMD4(CCC_Float, "r3_dynamic_wet_surfaces_far", &ps_r3_dyn_wet_surf_far, 30, 100);
	CMD4(CCC_Integer, "r3_dynamic_wet_surfaces_sm_res", &ps_r3_dyn_wet_surf_sm_res, 64, 2048);

	CMD3(CCC_Mask, "r3_volumetric_smoke", &ps_r2_ls_flags, R3FLAG_VOLUMETRIC_SMOKE);
	CMD1(CCC_memory_stats, "render_memory_stats");
}
