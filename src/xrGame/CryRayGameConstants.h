#pragma once
#include "actor.h"

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

			float af_koef();
			const float artefact_reac_value();

			virtual void save(NET_Packet& output_packet);
			virtual void load(IReader& input_packet);
	};

//	namespace ActorScriptSpace
//	{
//		bool IsCrouched() { return Actor() ? !!(Actor()->MovingState() & mcCrouch) : false; };
//		bool IsLowCrouched() { return Actor() ? (!!(Actor()->MovingState() & mcCrouch) && !!(Actor()->MovingState() & mcAccel)) : false; };
//		bool IsWalking() { return Actor() ? !!(Actor()->MovingState() & mcAnyMove) : false; };
//		bool IsSlowWalking() { return Actor() ? (!!(Actor()->MovingState() & mcAnyMove) && !!(Actor()->MovingState() & mcAccel)) : false; };
//		bool IsSprinting() { return Actor() ? !!(Actor()->MovingState() & mcSprint) : false; };
//		bool IsClimbing() { return Actor() ? !!(Actor()->MovingState() & mcClimb) : false; };
//	};
};
