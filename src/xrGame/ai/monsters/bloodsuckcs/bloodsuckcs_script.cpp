#include "pch_script.h"
#include "bloodsuckcs.h"

using namespace luabind;

#pragma optimize("s",on)
void CAI_Bloodsuckcs::script_register(lua_State* L)
{
	module(L)
		[
			class_<CAI_Bloodsuckcs, CGameObject>("CAI_Bloodsuckcs")
			.def(constructor<>())
		];
}
