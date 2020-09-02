#pragma once

#include "../monster_state_manager.h"

class CPoltergeist_cs;

class CStateManagerPoltergeist_cs : public CMonsterStateManager<CPoltergeist_cs> 
{
	typedef CMonsterStateManager<CPoltergeist_cs> inherited;

public:
						CStateManagerPoltergeist_cs		(CPoltergeist_cs *obj);
	virtual				~CStateManagerPoltergeist_cs	();

	virtual void		reinit						();
    virtual void        execute                     (); 
	virtual void		remove_links				(CObject* object) { inherited::remove_links(object);}

private:
			u32			time_next_flame_attack;
			u32			time_next_tele_attack;
			u32			time_next_scare_attack;

            CPolterSpecialAbility_cs* m_flame;
            CPolterSpecialAbility_cs* m_tele_cs; // Пересечение с ЗП классом
            IC CPolterSpecialAbility_cs* ability() { return (m_flame ? m_flame : m_tele_cs); }
};

