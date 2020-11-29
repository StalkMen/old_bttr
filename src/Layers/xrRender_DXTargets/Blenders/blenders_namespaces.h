#pragma once

namespace BLENDER // Пространство имен, общие блендеры -> BLENDER::
{
	class CBlender_bloom_build : public IBlender
	{
	public:
		virtual		LPCSTR		getComment() { return "Combine to bloom target"; }
		virtual		BOOL		canBeDetailed() { return FALSE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }

		virtual		void		Compile(CBlender_Compile& C);

		CBlender_bloom_build();
		virtual ~CBlender_bloom_build();
	};

	class CBlender_bloom_build_msaa : public IBlender
	{
	public:
		virtual		LPCSTR		getComment() { return "Combine to bloom target msaa"; }
		virtual		BOOL		canBeDetailed() { return FALSE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }

		virtual		void		Compile(CBlender_Compile& C);

		CBlender_bloom_build_msaa();
		virtual ~CBlender_bloom_build_msaa();
	};

	class CBlender_postprocess_msaa : public IBlender
	{
	public:
		virtual		LPCSTR		getComment() { return "Combine to bloom target msaa"; }
		virtual		BOOL		canBeDetailed() { return FALSE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }

		virtual		void		Compile(CBlender_Compile& C);

		CBlender_postprocess_msaa();
		virtual ~CBlender_postprocess_msaa();
	};

	class CBlender_combine : public IBlender
	{
	public:
		virtual		LPCSTR		getComment() { return "Combiner"; }
		virtual		BOOL		canBeDetailed() { return FALSE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }

		virtual		void		Compile(CBlender_Compile& C);

		CBlender_combine();
		virtual ~CBlender_combine();
	};

	class CBlender_combine_msaa : public IBlender
	{
	public:
		virtual		LPCSTR		getComment() { return "Combiner"; }
		virtual		BOOL		canBeDetailed() { return FALSE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }

		virtual		void		Compile(CBlender_Compile& C);

		CBlender_combine_msaa();
		virtual ~CBlender_combine_msaa();
		virtual   void    SetDefine(LPCSTR Name, LPCSTR Definition)
		{
			this->Name = Name;
			this->Definition = Definition;
		}
		LPCSTR Name;
		LPCSTR Definition;
	};

	class CBlender_deffer_aref : public IBlender
	{
	public:
		xrP_Integer	oAREF;
		xrP_BOOL	oBlend;
		bool		lmapped;
	public:
		virtual		LPCSTR		getComment() { return "Defer-base-aref"; }
		virtual		BOOL		canBeDetailed() { return TRUE; }
		virtual		BOOL		canBeLMAPped() { return lmapped; }
		virtual		BOOL		canUseSteepParallax() { return TRUE; }

		virtual		void		Save(IWriter& fs);
		virtual		void		Load(IReader& fs, u16 version);
		virtual		void		Compile(CBlender_Compile& C);

		CBlender_deffer_aref(bool _lmapped = false);
		virtual ~CBlender_deffer_aref();
	};

	class CBlender_deffer_flat : public IBlender
	{
	public:
		virtual		LPCSTR		getComment() { return "Defer-base-normal"; }
		virtual		BOOL		canBeDetailed() { return TRUE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }
		virtual		BOOL		canUseSteepParallax() { return TRUE; }

		virtual		void		Save(IWriter& fs);
		virtual		void		Load(IReader& fs, u16 version);
		virtual		void		Compile(CBlender_Compile& C);

		CBlender_deffer_flat();
		virtual ~CBlender_deffer_flat();

	private:
		xrP_TOKEN	oTessellation;
	};

	class CBlender_deffer_model : public IBlender
	{
	public:
		xrP_Integer	oAREF;
		xrP_BOOL	oBlend;
	public:
		virtual		LPCSTR		getComment() { return "Deffer-model-flat"; }
		virtual		BOOL		canBeDetailed() { return TRUE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }

		virtual		void		Save(IWriter& fs);
		virtual		void		Load(IReader& fs, u16 version);
		virtual		void		Compile(CBlender_Compile& C);

		CBlender_deffer_model();
		virtual ~CBlender_deffer_model();

	private:
		xrP_TOKEN	oTessellation;
	};

	class CBlender_luminance : public IBlender
	{
	public:
		virtual		LPCSTR		getComment() { return "Luminance estimate"; }
		virtual		BOOL		canBeDetailed() { return FALSE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }

		virtual		void		Compile(CBlender_Compile& C);

		CBlender_luminance();
		virtual ~CBlender_luminance();
	};

	class CBlender_accum_spot : public IBlender
	{
	public:
		virtual		LPCSTR	getComment() { return "Accumulate spot light"; }
		virtual		BOOL		canBeDetailed() { return FALSE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }

		virtual		void		Compile(CBlender_Compile& C);

		CBlender_accum_spot();
		virtual ~CBlender_accum_spot();
	};

	class CBlender_accum_spot_msaa : public IBlender
	{
	public:
		virtual		LPCSTR	getComment() { return "Accumulate spot light msaa"; }
		virtual		BOOL		canBeDetailed() { return FALSE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }

		virtual		void		Compile(CBlender_Compile& C);

		virtual   void    SetDefine(LPCSTR Name, LPCSTR Definition)
		{
			this->Name = Name;
			this->Definition = Definition;
		}

		CBlender_accum_spot_msaa();
		virtual ~CBlender_accum_spot_msaa();
		LPCSTR Name;
		LPCSTR Definition;
	};

	class CBlender_accum_volumetric_msaa : public IBlender
	{
	public:
		virtual		LPCSTR	getComment() { return "Accumulate spot light msaa"; }
		virtual		BOOL		canBeDetailed() { return FALSE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }

		virtual		void		Compile(CBlender_Compile& C);

		virtual   void    SetDefine(LPCSTR Name, LPCSTR Definition)
		{
			this->Name = Name;
			this->Definition = Definition;
		}

		CBlender_accum_volumetric_msaa();
		virtual ~CBlender_accum_volumetric_msaa();
		LPCSTR Name;
		LPCSTR Definition;
	};

	class CBlender_accum_reflected : public IBlender
	{
	public:
		virtual		LPCSTR		getComment() { return "Accumulate reflected light"; }
		virtual		BOOL		canBeDetailed() { return FALSE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }

		virtual		void		Compile(CBlender_Compile& C);

		CBlender_accum_reflected();
		virtual ~CBlender_accum_reflected();
	};

	class CBlender_accum_reflected_msaa : public IBlender
	{
	public:
		virtual		LPCSTR		getComment() { return "Accumulate reflected light"; }
		virtual		BOOL		canBeDetailed() { return FALSE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }

		virtual		void		Compile(CBlender_Compile& C);

		CBlender_accum_reflected_msaa();
		virtual ~CBlender_accum_reflected_msaa();
		virtual   void    SetDefine(LPCSTR Name, LPCSTR Definition)
		{
			this->Name = Name;
			this->Definition = Definition;
		}
		LPCSTR Name;
		LPCSTR Definition;
	};

	class CBlender_accum_point : public IBlender
	{
	public:
		virtual		LPCSTR		getComment() { return "Accumulate point light"; }
		virtual		BOOL		canBeDetailed() { return FALSE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }

		virtual		void		Compile(CBlender_Compile& C);

		CBlender_accum_point();
		virtual ~CBlender_accum_point();
	};

	class CBlender_accum_point_msaa : public IBlender
	{
	public:
		virtual		LPCSTR		getComment() { return "Accumulate point light msaa"; }
		virtual		BOOL		canBeDetailed() { return FALSE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }

		virtual		void		Compile(CBlender_Compile& C);

		CBlender_accum_point_msaa();
		virtual ~CBlender_accum_point_msaa();
		virtual   void    SetDefine(LPCSTR Name, LPCSTR Definition)
		{
			this->Name = Name;
			this->Definition = Definition;
		}
		LPCSTR Name;
		LPCSTR Definition;
	};

	class CBlender_light_occq : public IBlender
	{
	public:
		virtual		LPCSTR		getComment() { return "Occlusion"; }
		virtual		BOOL		canBeDetailed() { return FALSE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }

		virtual		void		Compile(CBlender_Compile& C);

		CBlender_light_occq();
		virtual ~CBlender_light_occq();
	};

	class CBlender_accum_direct_mask : public IBlender
	{
	public:
		virtual		LPCSTR		getComment() { return "Mask direct light"; }
		virtual		BOOL		canBeDetailed() { return FALSE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }

		virtual		void		Compile(CBlender_Compile& C);

		CBlender_accum_direct_mask();
		virtual ~CBlender_accum_direct_mask();
	};

	class CBlender_accum_direct_mask_msaa : public IBlender
	{
	public:

		virtual		LPCSTR	getComment() { return "Mask direct light msaa"; }
		virtual		BOOL		canBeDetailed() { return FALSE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }
		virtual  void     SetDefine(LPCSTR Name, LPCSTR Definition)
		{
			this->Name = Name;
			this->Definition = Definition;
		}

		virtual		void		Compile(CBlender_Compile& C);

		CBlender_accum_direct_mask_msaa();
		virtual ~CBlender_accum_direct_mask_msaa();

		LPCSTR Name;
		LPCSTR Definition;
	};

	class CBlender_accum_direct : public IBlender
	{
	public:
		virtual		LPCSTR	getComment() { return "Accumulate direct light"; }
		virtual		BOOL		canBeDetailed() { return FALSE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }

		virtual		void		Compile(CBlender_Compile& C);

		CBlender_accum_direct();
		virtual ~CBlender_accum_direct();
	};

	class CBlender_accum_direct_msaa : public IBlender
	{
	public:

		virtual		LPCSTR	getComment() { return "Accumulate direct light"; }
		virtual		BOOL		canBeDetailed() { return FALSE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }

		virtual		void		Compile(CBlender_Compile& C);
		virtual   void    SetDefine(LPCSTR Name, LPCSTR Definition)
		{
			this->Name = Name;
			this->Definition = Definition;
		}
		LPCSTR Name;
		LPCSTR Definition;

		CBlender_accum_direct_msaa();
		virtual ~CBlender_accum_direct_msaa();

	};

	class CBlender_accum_direct_volumetric_msaa : public IBlender
	{
	public:

		virtual		LPCSTR	getComment() { return "Accumulate direct light"; }
		virtual		BOOL		canBeDetailed() { return FALSE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }

		virtual		void		Compile(CBlender_Compile& C);
		virtual   void    SetDefine(LPCSTR Name, LPCSTR Definition)
		{
			this->Name = Name;
			this->Definition = Definition;
		}

		CBlender_accum_direct_volumetric_msaa();
		virtual ~CBlender_accum_direct_volumetric_msaa();
		LPCSTR Name;
		LPCSTR Definition;
	};

	class CBlender_accum_direct_volumetric_sun_msaa : public IBlender
	{
	public:

		virtual		LPCSTR	getComment() { return "Accumulate direct light"; }
		virtual		BOOL		canBeDetailed() { return FALSE; }
		virtual		BOOL		canBeLMAPped() { return FALSE; }

		virtual		void		Compile(CBlender_Compile& C);
		virtual   void    SetDefine(LPCSTR Name, LPCSTR Definition)
		{
			this->Name = Name;
			this->Definition = Definition;
		}

		CBlender_accum_direct_volumetric_sun_msaa();
		virtual ~CBlender_accum_direct_volumetric_sun_msaa();
		LPCSTR Name;
		LPCSTR Definition;
	};

	namespace AA // Пространство имен, сглаживания -> BLENDER::AA::
	{
		class CBlender_dlaa : public IBlender
		{
		public:
			virtual		LPCSTR		getComment() { return "DLAA"; }
			virtual		BOOL		canBeDetailed() { return FALSE; }
			virtual		BOOL		canBeLMAPped() { return FALSE; }

			virtual		void		Compile(CBlender_Compile& C);

			CBlender_dlaa();
			virtual ~CBlender_dlaa();
		};

		class CBlender_FXAA : public IBlender
		{
		public:
			virtual		LPCSTR		getComment() { return "FXAA"; }
			virtual		BOOL		canBeDetailed() { return FALSE; }
			virtual		BOOL		canBeLMAPped() { return FALSE; }

			virtual		void		Compile(CBlender_Compile& C);

			CBlender_FXAA();
			virtual ~CBlender_FXAA();
		};

		class CBlender_SMAA : public IBlender
		{
		public:
			virtual		LPCSTR		getComment() { return "SMAA"; }
			virtual		BOOL		canBeDetailed() { return FALSE; }
			virtual		BOOL		canBeLMAPped() { return FALSE; }

			virtual		void		Compile(CBlender_Compile& C);

			CBlender_SMAA();
			virtual ~CBlender_SMAA();
		};
	}

	namespace AO // Пространство имен, затенение картинки -> BLENDER::AO::
	{
		class CBlender_SSAO_noMSAA : public IBlender
		{
		public:
			virtual		LPCSTR		getComment() { return "SSAO"; }
			virtual		BOOL		canBeDetailed() { return FALSE; }
			virtual		BOOL		canBeLMAPped() { return FALSE; }

			virtual		void		Compile(CBlender_Compile& C);

			CBlender_SSAO_noMSAA();
			virtual ~CBlender_SSAO_noMSAA();
		};

		class CBlender_SSAO_MSAA : public IBlender
		{
		public:
			virtual		LPCSTR		getComment() { return "SSAO MSAA"; }
			virtual		BOOL		canBeDetailed() { return FALSE; }
			virtual		BOOL		canBeLMAPped() { return FALSE; }

			virtual		void		Compile(CBlender_Compile& C);

			CBlender_SSAO_MSAA();
			virtual ~CBlender_SSAO_MSAA();
			virtual   void    SetDefine(LPCSTR Name, LPCSTR Definition)
			{
				this->Name = Name;
				this->Definition = Definition;
			}
			LPCSTR Name;
			LPCSTR Definition;
		};
	}

	namespace GAME // Пространство имен, шейдеры улучшения гемплея -> BLENDER::GAME::
	{
		class CBlender_gamma : public IBlender
		{
		public:
			virtual		LPCSTR		getComment() { return "Gamma"; }
			virtual		BOOL		canBeDetailed() { return FALSE; }
			virtual		BOOL		canBeLMAPped() { return FALSE; }

			virtual		void		Compile(CBlender_Compile& C);

			CBlender_gamma();
			virtual ~CBlender_gamma();
		};

		class CBlender_sunshafts : public IBlender
		{
		public:
			virtual		LPCSTR		getComment() { return "Sunshafts"; }
			virtual		BOOL		canBeDetailed() { return FALSE; }
			virtual		BOOL		canBeLMAPped() { return FALSE; }

			virtual		void		Compile(CBlender_Compile& C);

			CBlender_sunshafts();
			virtual ~CBlender_sunshafts();
		};

		class CBlender_gasmask : public IBlender
		{
		public:
			virtual		LPCSTR		getComment() { return "Gasmask"; }
			virtual		BOOL		canBeDetailed() { return FALSE; }
			virtual		BOOL		canBeLMAPped() { return FALSE; }

			virtual		void		Compile(CBlender_Compile& C);

			CBlender_gasmask();
			virtual ~CBlender_gasmask();
		};

		class CBlender_rain_drops : public IBlender
		{
		public:
			virtual		LPCSTR		getComment() { return "Rain drops"; }
			virtual		BOOL		canBeDetailed() { return FALSE; }
			virtual		BOOL		canBeLMAPped() { return FALSE; }

			virtual		void		Compile(CBlender_Compile& C);

			CBlender_rain_drops();
			virtual ~CBlender_rain_drops();
		};
	}
}