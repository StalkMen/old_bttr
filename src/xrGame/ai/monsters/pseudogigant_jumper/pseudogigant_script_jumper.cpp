#include "pch_script.h"
#include "pseudo_gigant_jumper.h"

using namespace luabind;

#pragma optimize("s",on)
void CPseudoGigant_jumper::script_register(lua_State* L)
{
	module(L)
		[
			class_<CPseudoGigant_jumper, CGameObject>("CPseudoGigant_jumper")
			.def(constructor<>())
		];
}