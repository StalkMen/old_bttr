#include "pch_script.h"
#include "chimera_cs.h"

using namespace luabind;

#pragma optimize("s",on)
void CChimecs::script_register(lua_State* L)
{
	module(L)
		[
			class_<CChimecs, CGameObject>("CChimecs")
			.def(constructor<>())
		];
}