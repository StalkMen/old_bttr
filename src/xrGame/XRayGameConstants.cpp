#include "StdAfx.h"
#include "XRayGameConstants.h"
#include "GamePersistent.h"

int noviceRankStart_		= 0;
int experiencedRankStart_	= 0;
int veteranRankStart_		= 0;
int masterRankStart_		= 0;

namespace GameConstants
{
	void LoadConstants()
	{
		noviceRankStart_			= READ_IF_EXISTS(pSettings, r_s32, "game_relations", "novice_rank",			0);   //pSettings->r_s32("game_relations", "novice_start");
		experiencedRankStart_		= READ_IF_EXISTS(pSettings, r_s32, "game_relations", "experienced_rank",	300); //pSettings->r_s32("game_relations", "experienced_start");
		veteranRankStart_			= READ_IF_EXISTS(pSettings, r_s32, "game_relations", "veteran_rank",		600); //pSettings->r_s32("game_relations", "veteran_start");
		masterRankStart_			= READ_IF_EXISTS(pSettings, r_s32, "game_relations", "master_rank",			900); //pSettings->r_s32("game_relations", "master_start");

		Msg("# 1. [XRayGameConstants]: Loaded parameters by rank: Novice: %i, Experienced: %i, Veteran: %i, Master: %i", noviceRankStart_, experiencedRankStart_, veteranRankStart_, masterRankStart_);

		Msg("# Final. [XRayGameConstants]: Are Loaded!");
	}

	int GetNoviceRankStart()
	{
		return noviceRankStart_;
	}

	int	GetExperiencesRankStart()
	{
		return experiencedRankStart_;
	}

	int	GetVeteranRankStart()
	{
		return veteranRankStart_;
	}

	int	GetMasterRankStart()
	{
		return masterRankStart_;
	}
}