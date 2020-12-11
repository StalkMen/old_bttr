// OldSerpskiStaler
// Внутренний движковый конфиг

#include "StdAfx.h"
#include "CryRayGameConstants.h"
#include "GamePersistent.h"

signed int noviceRankStart = 0;
signed int experiencedRankStart = 0;
signed int veteranRankStart = 0;
signed int masterRankStart = 0;

u32 alife_sheduler_min = 0;
u32 alife_sheduler_max = 0;
u32 alife_object_per_update = 0;
u64 alife_sheduler_process_time = 0;

float alife_time_factor = 0.f;
float alife_update_monster_factor = 0.f;
float normal_time_factor = 0.f;
float switch_distance = 0.f;
float switch_factor = 0.f;

float artefact_koef_reaction = 0.f;

namespace CryRayParams 
{
	ENGINE_API CryRayClass CRParams;

	//Деструкторы класса
	CryRayClass::CryRayClass() {};
	CryRayClass::~CryRayClass() {}

	void CryRayCheckParams()
	{
		Msg("# 0.0. [CryRayParams]: Start game (+ Alife): schedule_min(% i), schedule_max(%i), process_time(%i), update_monster_factor(%f), time_factor(%f), objects_per_update(%i), normal_time_factor (%f), switch_distance (%f), switch_factor (%f)", 
			alife_sheduler_min, alife_sheduler_max, alife_sheduler_process_time, alife_update_monster_factor, alife_time_factor, alife_object_per_update, normal_time_factor, switch_distance, switch_factor);
		Msg("# 1.0. [CryRayParams]: Start game (+ NPC): Novice: (%i), Experienced: (%i), Veteran: (%i), Master: (%i)", noviceRankStart, experiencedRankStart, veteranRankStart, masterRankStart);
		Msg("# 2.0. [CryRayParams]: Start game (+ Artefact): artefact_koef_reaction (%f)", artefact_koef_reaction);
		Msg("# Final. [CryRayParams]: Check default params!");
	}

	void CryRayClass::LoadParameters()
	{
		// Ранги НПС по дефолту:
		noviceRankStart = READ_IF_EXISTS(pSettings, r_s32, "game_relations", "novice_rank", 0);
		experiencedRankStart = READ_IF_EXISTS(pSettings, r_s32, "game_relations", "experienced_rank", 300);
		veteranRankStart = READ_IF_EXISTS(pSettings, r_s32, "game_relations", "veteran_rank", 600);
		masterRankStart = READ_IF_EXISTS(pSettings, r_s32, "game_relations", "master_rank", 900);
		Msg("# 0. [CryRayParams]: Loaded parameters: Novice: (%i), Experienced: (%i), Veteran: (%i), Master: (%i)", noviceRankStart, experiencedRankStart, veteranRankStart, masterRankStart);

		// Файл alife.ltx и его параметры:
		alife_sheduler_min = READ_IF_EXISTS(pSettings, r_s32, "alife", "alife_schedule_min", 1); // милисекунд
		alife_sheduler_max = READ_IF_EXISTS(pSettings, r_s32, "alife", "alife_schedule_max", 1); // милисекунд
		alife_sheduler_process_time = READ_IF_EXISTS(pSettings, r_s64, "alife", "alife_process_time", 900); // милисекунд
		alife_update_monster_factor = READ_IF_EXISTS(pSettings, r_float, "alife", "alife_update_monster_factor", 0.1f);
		alife_time_factor = READ_IF_EXISTS(pSettings, r_float, "alife", "alife_time_factor", BttR_mode ? 4 : 10); // Скорость для демонстрации смены дня и ночи
		alife_object_per_update = READ_IF_EXISTS(pSettings, r_s32, "alife", "alife_objects_per_update", 20);
		normal_time_factor = READ_IF_EXISTS(pSettings, r_float, "alife", "normal_time_factor", 10.f); //животные в оффлайне перемещаются со скоростью, делённой на этот фактор (чтобы не бегали очень быстро)
		switch_distance = READ_IF_EXISTS(pSettings, r_float, "alife", "switch_distance", 250.f);
		switch_factor = READ_IF_EXISTS(pSettings, r_float, "alife", "switch_factor", 0.1f);
		Msg("# 1. [CryRayParams]: Loaded parameters: schedule_min (%i), schedule_max (%i), process_time (%i), update_monster_factor (%f), time_factor (%f), objects_per_update (%i), normal_time_factor (%f), switch_distance (%f), switch_factor (%f)",
			alife_sheduler_min, alife_sheduler_max, alife_sheduler_process_time, alife_update_monster_factor, alife_time_factor, alife_object_per_update, normal_time_factor, switch_distance, switch_factor);
		
		// Артефакты:
		artefact_koef_reaction = READ_IF_EXISTS(pSettings, r_float, "artefacts_cryray", "artefact_koef_reaction", 100.f);

		Msg("# 2. [CryRayParams]: Loaded parametrs: artefact_koef_reaction (%f)", artefact_koef_reaction);

		Msg("# Final. [CryRayParams]: Are Loaded!");
	}

	void CryRayClass::save(NET_Packet& output_packet)
	{
		Msg("! [%s]", __FUNCTION__);
	}

	void CryRayClass::load(IReader& input_packet)
	{
		Msg("! [%s]", __FUNCTION__);
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

	u32 CryRayClass::alife_object_per()
	{
		return alife_object_per_update;
	}

	u64 CryRayClass::alife_sheduler_process()
	{
		return alife_sheduler_process_time;
	}

	float CryRayClass::time_factor_normal() const
	{
		return normal_time_factor;
	}

	float CryRayClass::switch_distance_normal() const
	{
		return switch_distance;
	}

	float CryRayClass::switch_factor_normal() const
	{
		return switch_factor;
	}

	const float CryRayClass::alife_worldtime_factor() const
	{
		return alife_time_factor;
	}

	const float CryRayClass::alife_update_monster() const
	{
		return alife_update_monster_factor;
	}

	float CryRayClass::af_koef()
	{
		return artefact_koef_reaction;
	}
}