#include "pch_script.h"
#include "Burcs.h"
#include "xrScriptEngine/ScriptExporter.hpp"

using namespace luabind;

	SCRIPT_EXPORT(CBurcs, (CGameObject), 
		{ 
			module(luaState)
			[
				class_<CBurcs, CGameObject>("CBurcs").def(constructor<>())
			]; 
		});
