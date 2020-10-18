#pragma once

enum{
		AF_GODMODE			=(1<<0),
		AF_NO_CLIP			=(1<<1),
		AF_UNLIMITEDAMMO	=(1<<3),
		AF_RUN_BACKWARD		=(1<<4),
		AF_AUTOPICKUP		=(1<<5),
		AF_PSP				=(1<<6),
		AF_DYNAMIC_MUSIC	=(1<<7),
		AF_GODMODE_RT		=(1<<8),
		AF_IMPORTANT_SAVE	=(1<<9),
		AF_CROUCH_TOGGLE	=(1<<10),
		AF_USE_TRACERS		=(1<<11),
		AF_3DSCOPE_ENABLE	=(1<<12),
		AF_BOOL_DISABLE_MINIMAP =(1<<13),
};

extern Flags32	psActorFlags;
extern BOOL		GodMode	();	

extern int		psActorSleepTime;

