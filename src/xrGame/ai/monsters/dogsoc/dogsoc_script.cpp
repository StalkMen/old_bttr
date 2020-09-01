#include "pch_script.h"
#include "dogsoc.h"

using namespace luabind;

#pragma optimize("s",on)
void CAI_Dogsoc::script_register(lua_State* L)
{
	module(L)
		[
			class_<CAI_Dogsoc, CGameObject>("CAI_Dogsoc")
			.def(constructor<>())
		];
}