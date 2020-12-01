// OldSerpskiStaler
// Внутренний движковый конфиг

#include "StdAfx.h"
#include "CryRayGameConstants.h"
#include "GamePersistent.h"

namespace CryRayParams 
{
	ENGINE_API CryRayClass CRParams;

	//Деструкторы класса
	CryRayClass::CryRayClass() {};
	CryRayClass::~CryRayClass() {}

	void CryRayClass::LoadParameters()
	{
		// Ранги НПС по дефолту:
		noviceRankStart = READ_IF_EXISTS(pSettings, r_s32, "game_relations", "novice_rank", 0);
		experiencedRankStart = READ_IF_EXISTS(pSettings, r_s32, "game_relations", "experienced_rank", 300);
		veteranRankStart = READ_IF_EXISTS(pSettings, r_s32, "game_relations", "veteran_rank", 600);
		masterRankStart = READ_IF_EXISTS(pSettings, r_s32, "game_relations", "master_rank", 900);
		Msg("# 1. [CryRayParams]: Loaded parameters: Novice: (%i), Experienced: (%i), Veteran: (%i), Master: (%i)", noviceRankStart, experiencedRankStart, veteranRankStart, masterRankStart);

		// Файл alife.ltx и его параметры:
		alife_sheduler_min = READ_IF_EXISTS(pSettings, r_s32, "alife", "alife_schedule_min", 1);
		alife_sheduler_max = READ_IF_EXISTS(pSettings, r_s32, "alife", "alife_schedule_max", 1);
		alife_sheduler_process_time = READ_IF_EXISTS(pSettings, r_s64, "alife", "alife_process_time", 900);
		alife_update_monster_factor = READ_IF_EXISTS(pSettings, r_float, "alife", "alife_update_monster_factor", 0.1f);
		alife_time_factor = READ_IF_EXISTS(pSettings, r_s32, "alife", "alife_time_factor", 4); // Скорость для демонстрации смены дня и ночи

		Msg("# 2. [CryRayParams]: Loaded parameters: schedule_min (%i), schedule_max (%i), process_time (%i), update_monster_factor (%i), time_factor (%i)", 
			alife_sheduler_min, alife_sheduler_max, alife_sheduler_process_time, alife_update_monster_factor, alife_time_factor);

		Msg("# Final. [CryRayParams]: Are Loaded!");
	}

	int CryRayClass::GetNoviceRankStart()
	{
		return noviceRankStart;
	}

	int	CryRayClass::GetExperiencesRankStart()
	{
		return experiencedRankStart;
	}

	int	CryRayClass::GetVeteranRankStart()
	{
		return veteranRankStart;
	}

	int	CryRayClass::GetMasterRankStart()
	{
		return masterRankStart;
	}

	u32 CryRayClass::alife_sheduler_min_factor()
	{
		return alife_sheduler_min;
	}

	u32 CryRayClass::alife_sheduler_max_factor()
	{
		return alife_sheduler_max;
	}

	u32 CryRayClass::alife_worldtime_factor()
	{
		return alife_time_factor;
	}

	u64 CryRayClass::alife_sheduler_process()
	{
		return alife_sheduler_process_time;
	}

	const float CryRayClass::alife_update_monser() const
	{
		return alife_update_monster_factor;
	}
}