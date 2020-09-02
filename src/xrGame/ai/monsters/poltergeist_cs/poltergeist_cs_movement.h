#pragma once

#include "../control_path_builder.h"

class CPoltergeist_cs;
class CCustomMonster;

class CPoltergeisMovementManager_cs : public CControlPathBuilder {
	typedef CControlPathBuilder inherited;

	CPoltergeist_cs	*m_monster;

public:
					CPoltergeisMovementManager_cs	(CPoltergeist_cs *monster) : inherited((CCustomMonster*)monster), m_monster(monster) {}
	virtual			~CPoltergeisMovementManager_cs	(){}


	virtual	void	move_along_path				(CPHMovementControl *movement_control, Fvector &dest_position, float time_delta);

			Fvector	CalculateRealPosition		();
};


