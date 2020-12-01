#pragma once

namespace CryRayParams
{
	class CryRayClass
	{
		private:
			signed int noviceRankStart = 0;
			signed int experiencedRankStart = 0;
			signed int veteranRankStart = 0;
			signed int masterRankStart = 0;

			u32 alife_sheduler_min = 0;
			u32 alife_sheduler_max = 0;
			u32 alife_time_factor = 0;

			u64 alife_sheduler_process_time = 0;

			float alife_update_monster_factor = 0.f;

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
			u32 alife_worldtime_factor();

			u64 alife_sheduler_process();

			const float alife_update_monser() const;
	};
};
