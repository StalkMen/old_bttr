#include "pch_script.h"
#include "bocs.h"

using namespace luabind;

#pragma optimize("s",on)
void CAI_CBocs::script_register(lua_State* L)
{
	module(L)
		[
			class_<CAI_CBocs, CGameObject>("CAI_CBocs")
			.def(constructor<>())
		];
}