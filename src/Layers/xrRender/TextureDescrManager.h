#pragma once
#include "ETextureParams.h"
#include "../../build_render_config.h"

class cl_dt_scaler;

#ifdef NEW_LOADING_TEXTURES
class cl_gloss_coef_and_offset : public R_constant_setup
{
public:
	float				coef;
	float				offset;

	cl_gloss_coef_and_offset(float s, float x) : coef(s), offset(x) {};
	virtual void setup(R_constant* C)
	{
		RCache.set_c(C, coef, offset, 0, 0);
	}
};
#endif

class CTextureDescrMngr
{
	struct texture_assoc
	{
		shared_str detail_name;
		u8 usage;
#ifdef NEW_LOADING_TEXTURES
		u8 m_tesselation_method;
#endif
#ifndef NEW_LOADING_TEXTURES
		texture_assoc() : usage(0) {}
#else
		texture_assoc() : usage(0) { m_tesselation_method = 32; }
#endif
		~texture_assoc		() {}

	};
	struct texture_spec
	{
		shared_str			m_bump_name;
		float				m_material;
		bool				m_use_steep_parallax;
#ifdef NEW_LOADING_TEXTURES
		cl_gloss_coef_and_offset* textureglossparams;
		texture_spec() { textureglossparams = NULL; }
		~texture_spec() { xr_delete(textureglossparams); }
#endif
	};
	struct texture_desc{
		texture_assoc*		m_assoc;
		texture_spec*		m_spec;
        texture_desc            ():m_assoc(NULL),m_spec(NULL){}
	};
	DEFINE_MAP(shared_str, texture_desc,	map_TD,	map_TDIt);
	DEFINE_MAP(shared_str, cl_dt_scaler*,	map_CS,	map_CSIt);

	map_TD									m_texture_details;
	map_CS									m_detail_scalers;

#ifndef NEW_LOADING_TEXTURES
	void LoadTHM(LPCSTR initial);
#else
	void LoadLTX();
	void LoadTHM();
	void LoadMiniLTX();
	void CheckAndCreate_Assoc(texture_desc*& desc);
	void CheckAndCreate_Spec(texture_desc*& desc);
#endif

public:
				~CTextureDescrMngr();
	void		Load		();
	void		UnLoad		();
public:
	shared_str	GetBumpName		(const shared_str& tex_name) const;
	float		GetMaterial		(const shared_str& tex_name) const;
	void		GetTextureUsage(const shared_str& tex_name, BOOL& bDiffuse, BOOL& bBump) const;
	BOOL		GetDetailTexture(const shared_str& tex_name, LPCSTR& res, R_constant_setup* &CS) const;
	BOOL		UseSteepParallax(const shared_str& tex_name) const;
#ifdef NEW_LOADING_TEXTURES
	u8 TessMethod(const shared_str& tex_name) const;
	void GetGlossParams(const shared_str& tex_name, R_constant_setup*& GlossParams) const;

private:
	mutable xrCriticalSection	protectTxDescPool_; // mutable, because it needs to be changed in const fuctions

	IC map_TD::const_iterator	FindInTxDetailsPool(const shared_str& tex_name) const; // mt access protection
	IC map_TD::const_iterator	GetTxDetailsPoolEndIter() const; // mt access protection
	IC void						LockTxDecsMutex();
	IC void						UnLockTxDecsMutex();
#endif
};