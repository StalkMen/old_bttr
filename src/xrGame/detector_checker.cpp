#include "stdafx.h"
#include "detector_checker.h"
#include "Actor.h"
#include "torch.h"
#include "script_engine.h"
#include "ai_space.h"

CDetectorChecker::CDetectorChecker()
{
}

CDetectorChecker::~CDetectorChecker()
{
}

BOOL CDetectorChecker::net_Spawn(CSE_Abstract* DC) 
{
	return		(inherited::net_Spawn(DC));
}

void CDetectorChecker::Load(LPCSTR section) 
{
	inherited::Load(section);
}

void CDetectorChecker::net_Destroy() 
{
	inherited::net_Destroy();
}

void CDetectorChecker::shedule_Update(u32 dt) 
{
	inherited::shedule_Update(dt);

}

void CDetectorChecker::UpdateCL() 
{
	inherited::UpdateCL();
}

void CDetectorChecker::OnH_A_Chield() 
{
	inherited::OnH_A_Chield		();
}

void CDetectorChecker::OnH_B_Independent(bool just_before_destroy) 
{
	inherited::OnH_B_Independent(just_before_destroy);
}

void CDetectorChecker::renderable_Render() 
{
	inherited::renderable_Render();
}

bool CDetectorChecker::UseBy (CEntityAlive* entity_alive)
{
//	CInventoryOwner *IO				= smart_cast<CInventoryOwner*>(entity_alive);
//	CActor			*actor			= NULL;

//	R_ASSERT						(IO);

//	actor							= smart_cast<CActor*>(IO);
//	R_ASSERT						(actor);
/*
	float get						= Actor()->fArtReac;
	LPCSTR							DetectorChecker;
	LUA_EXPORT						functor;

	if (get == 0.f)
	{
		R_ASSERT(_SCRIPT_ENGINE("_export_cryray.detector_checker_lvl0", functor));
		DetectorChecker = functor();
	}
	else if ((get < 0.15f) && (get > 0.0f))
	{
		R_ASSERT(_SCRIPT_ENGINE("_export_cryray.detector_checker_lvl1", functor));
		DetectorChecker = functor();
	}
	else if ((get < 0.30f) && (get > 0.15f))
	{
		R_ASSERT(_SCRIPT_ENGINE("_export_cryray.detector_checker_lvl2", functor));
		DetectorChecker = functor();
	}
	else if ((get < 0.45f) && (get > 0.30f))
	{
		R_ASSERT(_SCRIPT_ENGINE("_export_cryray.detector_checker_lvl3", functor));
		DetectorChecker = functor();
	}
	else if ((get < 0.60f) && (get > 0.45f))
	{
		R_ASSERT(_SCRIPT_ENGINE("_export_cryray.detector_checker_lvl4", functor));
		DetectorChecker = functor();
	}
	*/
	bool used = inherited::UseBy(entity_alive);
	return used;
}

bool CDetectorChecker::Empty() const
{
	return inherited::Empty();
}