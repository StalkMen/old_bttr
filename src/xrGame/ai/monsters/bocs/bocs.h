#pragma once
#include "../BaseMonster/base_monster.h"

class CStateManagerBocs;

class CAI_CBocs : public CBaseMonster {
	typedef		CBaseMonster		inherited;
	
public:
					CAI_CBocs 			();
	virtual			~CAI_CBocs 			();

	virtual void	Load				(LPCSTR section);
	virtual void	CheckSpecParams		(u32 spec_params);
	
	virtual void	reinit				();
	
	virtual	char* get_monster_class_name() { return "bocs"; }

	DECLARE_SCRIPT_REGISTER_FUNCTION
};

add_to_type_list(CAI_CBocs)
#undef script_type_list
#define script_type_list save_type_list(CAI_CBocs)