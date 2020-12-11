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
	LPCSTR detector_checker;
	LUA_EXPORT read_only;
	R_ASSERT(_SCRIPT_ENGINE("_export_cryray.detector_used", read_only));
	detector_checker = read_only();

	bool used = inherited::UseBy(entity_alive);
	return used;
}

bool CDetectorChecker::Empty() const
{
	CTorch *flashlight = Actor()->GetCurrentTorch();
	if (!flashlight)
		return false;

	return inherited::Empty();
}