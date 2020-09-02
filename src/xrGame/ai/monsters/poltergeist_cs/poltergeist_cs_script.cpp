#include "pch_script.h"
#include "poltergeist_cs.h"

using namespace luabind;

#pragma optimize("s",on)
void CPoltergeist_cs::script_register(lua_State* L)
{
	module(L)
		[
			class_<CPoltergeist_cs, CGameObject>("CPoltergeist_cs")
			.def(constructor<>())
		];
}