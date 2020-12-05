// Texture.cpp: implementation of the CTexture class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#pragma hdrstop

#pragma warning(disable:4995)
#include <d3dx9.h>
#pragma warning(default:4995)

#include <D3DX10Tex.h>

#include "../xrRender/dxRenderDeviceRender.h"
#include "../../build_render_config.h"
#include "../xrRenderDX10/dx10TextureDDS.h"

constexpr cpcstr NOT_EXISTING_TEXTURE = "ed\\ed_not_existing_texture";

void fix_texture_name(LPSTR fn)
{
	LPSTR _ext = strext(fn);
	if(  _ext					&&
		(0==stricmp(_ext,".tga")	||
		0==stricmp(_ext,".dds")	||
		0==stricmp(_ext,".bmp")	||
		0==stricmp(_ext,".ogm")	) )
		*_ext = 0;
}

int get_texture_load_lod(LPCSTR fn)
{
#ifndef DISABLE_LOD_MANAGER
	CInifile::Sect& sect	= pSettings->r_section("reduce_lod_texture_list");
	CInifile::SectCIt it_	= sect.Data.begin();
	CInifile::SectCIt it_e_	= sect.Data.end();
#endif

	ENGINE_API bool is_enough_address_space_available();
	static bool enough_address_space_available = is_enough_address_space_available();

#ifndef DISABLE_LOD_MANAGER
	CInifile::SectCIt it	= it_;
	CInifile::SectCIt it_e	= it_e_;

	for(;it!=it_e;++it)
	{
		if( strstr(fn, it->first.c_str()) )
		{
			if(psTextureLOD<1) {
				if ( enough_address_space_available )
					return 0;
				else
					return 1;
			}
			else
				if(psTextureLOD<3)
					return 1;
				else
					return 2;
		}
	}

	if(psTextureLOD<2) 
	{
		return 0;
	}
#endif

#ifdef DISABLE_LOD_MANAGER
	if (enough_address_space_available)
		return psTextureLOD % 2 == 0 ? psTextureLOD / 2 : 0;
#endif
	else
#ifndef DISABLE_LOD_MANAGER
		if (psTextureLOD < 4)
			return 1;
		else
			return 2;
#else
	return 2;
#endif
}

u32 calc_texture_size(int lod, u32 mip_cnt, u32 orig_size)
{
	if(1==mip_cnt)
		return orig_size;

	int _lod		= lod;
	float res		= float(orig_size);

	while(_lod>0){
		--_lod;
		res		-= res/1.333f;
	}
	return iFloor	(res);
}

const float		_BUMPHEIGH = 8.f;
//////////////////////////////////////////////////////////////////////
// Utility pack
//////////////////////////////////////////////////////////////////////
IC u32 GetPowerOf2Plus1	(u32 v)
{
	u32 cnt=0;
	while (v) {v>>=1; cnt++; };
	return cnt;
}
IC void	Reduce				(int& w, int& h, int& l, int& skip)
{
	while ((l>1) && skip)
	{
		w /= 2;
		h /= 2;
		l -= 1;

		skip--;
	}
	if (w<1)	w=1;
	if (h<1)	h=1;
}

IC void	Reduce(UINT& w, UINT& h, int l, int skip)
{
	while ((l>1) && skip)
	{
		w /= 2;
		h /= 2;
		l -= 1;

		skip--;
	}
	if (w<1)	w=1;
	if (h<1)	h=1;
}

void				TW_Save(ID3DTexture2D* T, LPCSTR name, LPCSTR prefix, LPCSTR postfix)
{
	string256		fn;		strconcat(sizeof(fn), fn, name, "_", prefix, "-", postfix);
	for (int it = 0; it<int(xr_strlen(fn)); it++)
		if ('\\' == fn[it])	fn[it] = '_';
	string256		fn2;	strconcat(sizeof(fn2), fn2, "debug\\", fn, ".dds");
	Log("! DX11 debug texture save: ", fn2);
	R_CHK(D3DX11SaveTextureToFile(DEVICE_HW::CRYRAY_RENDER::HW.pRenderContext, T, D3DX11_IFF_DDS, fn2));
}

ID3DBaseTexture*	CRender::texture_load(LPCSTR fRName, u32& ret_msize, bool bStaging)
{
	//	Moved here just to avoid warning
	D3DX11_IMAGE_INFO			IMG;
	ZeroMemory(&IMG, sizeof(IMG));

	//	Staging control
	static bool bAllowStaging = !RImplementation.o.no_ram_textures;
	bStaging &= bAllowStaging;

	ID3DBaseTexture*		pTexture2D		= NULL;
	//IDirect3DCubeTexture9*	pTextureCUBE	= NULL;
	string_path				fn;
	//u32						dwWidth,dwHeight;
	u32						img_size		= 0;
	int						img_loaded_lod	= 0;
	//D3DFORMAT				fmt;
	u32						mip_cnt=u32(-1);
	// validation
	R_ASSERT				(fRName);
	R_ASSERT				(fRName[0]);

	// make file name
	string_path				fname;
	xr_strcpy(fname,fRName); //. andy if (strext(fname)) *strext(fname)=0;
	fix_texture_name		(fname);
	XRayDDSLoader			TextureLoader;
	IReader* S				= NULL;
	//if (!FS.exist(fn,"$game_textures$",	fname,	".dds")	&& strstr(fname,"_bump"))	goto _BUMP_from_base;
	if (strstr(fname,"_bump")) 
	{
		if (!FS.exist(fn,"$game_textures$",	fname,	".dds"))
			goto _BUMP_from_base;
		else if (strstr(Core.Params,"-no_bump_mode2"))
		{
			if (strstr(fname,"_bump#"))
			{
				R_ASSERT2	(FS.exist(fn,"$game_textures$",	"ed\\ed_dummy_bump#",	".dds"), "ed_dummy_bump#");
				S						= FS.r_open	(fn);
				R_ASSERT2				(S, fn);
				img_size				= S->length	();
				goto		_DDS_2D;
			}
		
			R_ASSERT2	(FS.exist(fn,"$game_textures$",	"ed\\ed_dummy_bump",	".dds"),"ed_dummy_bump");
			S						= FS.r_open	(fn);

			R_ASSERT2	(S, fn);

			img_size				= S->length	();
			goto		_DDS_2D;
		}
		else if (strstr(Core.Params,"-no_bump_mode1") && strstr(fname,"_bump#"))
		{
			R_ASSERT2	(FS.exist(fn,"$game_textures$",	"ed\\ed_dummy_bump#",	".dds"), "ed_dummy_bump#");
			S						= FS.r_open	(fn);
			R_ASSERT2				(S, fn);
			img_size				= S->length	();
			goto		_DDS_2D;
		}
	} 	
	if (FS.exist(fn,"$level$",			fname,	".dds"))							goto _DDS;
	if (FS.exist(fn,"$game_saves$",		fname,	".dds"))							goto _DDS;
	if (FS.exist(fn,"$game_textures$",	fname,	".dds"))							goto _DDS;

CANT_LOAD:
	{
		Msg("! DX11 Can't find texture '%s'", fname);
	}
	const bool dummyTextureExist = FS.exist(fn, "$game_textures$", NOT_EXISTING_TEXTURE, ".dds");
#pragma todo("OldSerpskiStalker: Fix crash dx11Textures")
	R_ASSERT3(dummyTextureExist, "! [DX11]: Fake texture does not exist, everything is normal. The render continues...", NOT_EXISTING_TEXTURE);
	if (!dummyTextureExist)
		return nullptr;

	goto _DDS;

_DDS:
	{
		// Load and get header
		if (ps_r__common_flags.test(RFLAGDX_OLD_LOAD_DDS))
		{
			S = FS.r_open(fn);
#ifdef DEBUG
			Msg("* Loaded: %s[%d]", fn, S->length());
#endif // DEBUG
			img_size = S->length();
			R_ASSERT(S);
			R_CHK2(D3DX11GetImageInfoFromMemory(S->pointer(), S->length(), 0, &IMG, 0), fn);
			if (IMG.MiscFlags & D3D_RESOURCE_MISC_TEXTURECUBE)			goto _DDS_CUBE;
			else														goto _DDS_2D;
		}
else
		{
			if (ps_r__common_flags.test(RFLAGDX_ENABLE_DEBUG_LOG))
				Msg("- [Debug]: Load Texture DX11:%s", fn);

			if (!TextureLoader.Load(fn))
			{
				if (ps_r__common_flags.test(RFLAGDX_ENABLE_DEBUG_LOG))
					Msg("- [Debug]: Can't load texture DX11:%s", fn);
				goto CANT_LOAD;
			}
			else
			{
				if (TextureLoader.isCube())
				{
					if (ps_r__common_flags.test(RFLAGDX_ENABLE_DEBUG_LOG))
						Msg("- [Debug]: Cube texture DX11:%s", fn);
					goto _DDS_CUBE;
				}
				else
				{
					goto _DDS_2D;
				}
			}
		}
	_DDS_CUBE:
		{
			//	Inited to default by provided default constructor
			if (ps_r__common_flags.test(RFLAGDX_OLD_LOAD_DDS))
			{
				D3DX11_IMAGE_LOAD_INFO LoadInfo;
				if (bStaging)
				{
					LoadInfo.Usage = D3D_USAGE_STAGING;
					LoadInfo.BindFlags = 0;
					LoadInfo.CpuAccessFlags = D3D_CPU_ACCESS_WRITE;
				}
				else
				{
					LoadInfo.Usage = D3D_USAGE_DEFAULT;
					LoadInfo.BindFlags = D3D_BIND_SHADER_RESOURCE;
				}

				LoadInfo.pSrcInfo = &IMG;

				R_CHK(D3DX11CreateTextureFromMemory(DEVICE_HW::CRYRAY_RENDER::HW.pRenderDevice, S->pointer(), S->length(), &LoadInfo, 0, &pTexture2D, 0));

				FS.r_close(S);

				// OK
				mip_cnt = IMG.MipLevels;
				ret_msize = calc_texture_size(img_loaded_lod, mip_cnt, img_size);
			}
else
			{
				ID3D11Texture2D* ptr = 0;
				TextureLoader.To(ptr, bStaging);
				ret_msize = TextureLoader.GetSizeInMemory();
				pTexture2D = ptr;
			}

			return pTexture2D;
		}
	_DDS_2D:
		{
			if (ps_r__common_flags.test(RFLAGDX_OLD_LOAD_DDS))
			{
				// Check for LMAP and compress if needed
				strlwr(fn);

				img_loaded_lod = get_texture_load_lod(fn);

				//	Inited to default by provided default constructor
				D3DX11_IMAGE_LOAD_INFO LoadInfo;
				LoadInfo.Width = IMG.Width;
				LoadInfo.Height = IMG.Height;

				// x64 crash workaround
#pragma TODO("OldSerpskiStalker, исправление вылета с текстурами на х64")
				LoadInfo.FirstMipLevel = img_loaded_lod;

#pragma TODO("OldSerpskiStalker, по совету Zagolski")
				LoadInfo.MipLevels = IMG.MipLevels;

				if (bStaging)
				{
					LoadInfo.Usage = D3D_USAGE_STAGING;
					LoadInfo.BindFlags = 0;
					LoadInfo.CpuAccessFlags = D3D_CPU_ACCESS_WRITE;
				}
				else
				{
					LoadInfo.Usage = D3D_USAGE_DEFAULT;
					LoadInfo.BindFlags = D3D_BIND_SHADER_RESOURCE;
				}
				LoadInfo.pSrcInfo = &IMG;

				R_CHK2(D3DX11CreateTextureFromMemory(DEVICE_HW::CRYRAY_RENDER::HW.pRenderDevice, S->pointer(), S->length(), &LoadInfo, 0, &pTexture2D, 0), fn);

				FS.r_close(S);
				mip_cnt = IMG.MipLevels;
				// OK
				ret_msize = calc_texture_size(img_loaded_lod, mip_cnt, img_size);
			}
else
			{
				ID3D11Texture2D* ptr = 0;
				TextureLoader.To(ptr, bStaging);
				ret_msize = TextureLoader.GetSizeInMemory();
				pTexture2D = ptr;
			}

			return					pTexture2D;
		}
	}

_BUMP_from_base:
	{
		//Msg			("! auto-generated bump map: %s",fname);
		Msg("! Fallback to default bump map DX11: %s", fname);
		//////////////////
		if (strstr(fname, "_bump#"))
		{
			R_ASSERT2(FS.exist(fn, "$game_textures$", "ed\\ed_dummy_bump#", ".dds"), "ed_dummy_bump#");
			S = FS.r_open(fn);
			R_ASSERT2(S, fn);
			if (ps_r__common_flags.test(RFLAGDX_OLD_LOAD_DDS))
			{}
			else
			{
				img_size = S->length();
				goto		_DDS_2D;
			}
		}
		if (strstr(fname, "_bump"))
		{
			R_ASSERT2(FS.exist(fn, "$game_textures$", "ed\\ed_dummy_bump", ".dds"), "ed_dummy_bump");
			S = FS.r_open(fn);

			R_ASSERT2(S, fn);
			if (ps_r__common_flags.test(RFLAGDX_OLD_LOAD_DDS))
			{}
			else
			{
				img_size = S->length();
				goto		_DDS_2D;
			}
		}
		//////////////////
			}

	return 0;
}
