#pragma once
#include "../states/monster_state_attack.h"

template<typename _Object>
class	CBloodsucksocStateAttack : public CStateMonsterAttack<_Object> {
	typedef CStateMonsterAttack<_Object> inherited_attack;

	u32				m_time_stop_invis;
	Fvector			m_dir_point;

public:
					CBloodsucksocStateAttack		(_Object *obj);
	virtual			~CBloodsucksocStateAttack	();

	virtual	void	initialize					();
	virtual	void	execute						();
	virtual	void	finalize					();
	virtual	void	critical_finalize			();
	
	virtual void	setup_substates				();
private:
			void	update_invisibility			();
			bool	check_hiding				();
			bool	check_vampire				();
};

#include "bloodsucksoc_attack_state_inline.h"
