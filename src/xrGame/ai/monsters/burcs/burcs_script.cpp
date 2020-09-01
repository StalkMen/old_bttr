#include "pch_script.h"
#include "Burcs.h"

using namespace luabind;

#pragma optimize("s",on)
void CBurcs::script_register(lua_State* L)
{
	module(L)
		[
			class_<CBurcs, CGameObject>("CBurcs")
			.def(constructor<>())
		];
}
