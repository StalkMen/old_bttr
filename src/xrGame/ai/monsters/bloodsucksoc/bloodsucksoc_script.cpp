#include "pch_script.h"
#include "bloodsucksoc.h"

using namespace luabind;

#pragma optimize("s",on)
void CAI_Bloodsucksoc::script_register(lua_State* L)
{
	module(L)
		[
			class_<CAI_Bloodsucksoc, CGameObject>("CAI_Bloodsucksoc")
			.def(constructor<>())
		];
}