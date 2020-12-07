#pragma once

namespace CryRayParams
{
	void CryRayCheckParams();

	class CryRayClass
	{
		public:

			CryRayClass();
			~CryRayClass();

			void LoadParameters();

			int	GetNoviceRankStart();
			int	GetExperiencesRankStart();
			int	GetVeteranRankStart();
			int	GetMasterRankStart();

			u32 alife_sheduler_min_factor();
			u32 alife_sheduler_max_factor();
			u32 alife_object_per();

			u64 alife_sheduler_process();
			
			float time_factor_normal() const;
			float switch_distance_normal() const;
			float switch_factor_normal() const;

			const float alife_worldtime_factor() const;
			const float alife_update_monster() const;

			virtual void CryRayClass::save(NET_Packet& output_packet);
			virtual void CryRayClass::load(IReader& input_packet);
	};
};
