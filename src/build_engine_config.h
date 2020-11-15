#pragma once
//-' Глобальный конфиг для дефайнов по всему движку (включая рендеры)

//-' TouchOfRay Engine x64
	#define CRASH_PROTECTION_OGSR(cond, ...) do { if (!(cond)) Msg(__VA_ARGS__); } while (0)	// Защита от вылета, когда аргумент равен нулю или исключению(DEBUG), OGSR +
	#define LUA_EXPORT					luabind::functor<LPCSTR>								// Для экспорта С++ в LUA вызовы
	#define _SCRIPT_ENGINE				ai().script_engine().functor							// ai().script_engine().functor
	#define NEW_AI_POLTER																		// Новая логика полтергейста ЧН
//	#define ACTIVE_VERIFY																	    // Активировать режим точности ошибок в релизе
	#define NEW_CPU_LOAD												  					    // Mini-rework choice of processor cores
	#define MULTICORE_ENGINE													  		        // Многоядерность процессора, снятие ограничения привязки на 1 ядро
	#define FPS_LIMIT																			// Новый лимит FPS
	#define FIX_GAME_INFO																		// Fix crash for fake section 'game_info'

	//-' Миниреворк загрузочных сообщений
	#define STAGE_1					"st_loading_saved_game_solyanka"			  // xrGame	+
	#define STAGE_2					"st_creating_new_game_solyanka"				  // xrGame +
	#define STAGE_3					"st_loading_alife_simulator_solyanka"		  // xrGame +
	//#define STAGE_4				"st_server_connecting_solyanka"				  // xrGame -
	#define STAGE_5					"st_client_synchronising_solyanka"			  // xrGame +
	#define STAGE_6					"st_loading_ai_objects_solyanka"			  // xrGame +
	#define STAGE_7					"st_client_spawning_solyanka"				  // xrGame +
	#define STAGE_8					"st_loading_textures_solyanka"				  // xrGame + 
	#define STAGE_9					"st_client_synchronising_solyanka"			  // xrGame (?) +
	#define STAGE_10				"st_server_starting_solyanka"				  // xrGame +
	#define STAGE_11				"st_loading_shaders_solyanka"				  // xrRender + 
	#define STAGE_12				"st_loading_geometry_solyanka"				  // xrRender +
	#define STAGE_13				"st_loading_spatial_db_solyanka"			  // xrRender +
	#define STAGE_14				"st_loading_details_solyanka"				  // xrRender +
	//#define STAGE_15				"st_loading_sectors_portals_solyanka"         // xrRender -
	#define STAGE_16				"st_loading_lights_solyanka"				  // xrRender +
	//#define STAGE_17				"st_prefetching_objects_solyanka"			  // xrEngine -
	#define STAGE_18				"st_loading_cform_solyanka"				      // xrEngine +

	#include <regex>
	#include <string>
//	#include <bitset>
//-' ...

// CALLBACKS
	#define EXTENDED_ITEM_CALLBACKS							// (eItemToBelt, eItemToSlot, eItemToRuck)
	#define EXTENDED_WEAPON_CALLBACKS						// (eOnWeaponZoomIn, eOnWeaponZoomOut, eOnWeaponJammed)
	#define INVENTORY_UPGRADE_CALLBACKS						// calls lua function inventory_upgrades.effect_upgrade_item
	#define ACTOR_BEFORE_DEATH_CALLBACK						// For extending the life of the actor to fake death or do other tasks that need to happen before actor is dead
	#define INPUT_CALLBACKS									// (eKeyPress, eKeyRelease, eKeyHold)
	//#define MOUSE_INPUT_CALLBACKS							// (eMouseMove, eMouseWheel)
	#define ENGINE_LUA_ALIFE_STORAGE_MANAGER_CALLBACKS		// calls lua functions from engine in a script named alife_storage_manager.script  (alife_storage_manager.CALifeStorageManager_save) and (alife_storage_manager.CALifeStorageManager_load)
	#define ENGINE_LUA_ALIFE_UPDAGE_MANAGER_CALLBACKS		// calls lua function named on_before_change_level and on_after_new_game in _G.script when enabled
//-CALLBACKS

// SCRIPTS:
	#define GAME_OBJECT_EXTENDED_EXPORTS					// see: script_game_object*.cpp/h
	#define GAME_OBJECT_TESTING_EXPORTS						// see: script_game_object4.cpp  functions for object testing (ie. is_stalker(), is_heli())
	#define NAMESPACE_LEVEL_EXPORTS							// see: level_script.cpp
	#define INI_FILE_EXTENDED_EXPORTS						// see: script_ini_file_script.cpp
	#define ENABLE_CAR										// reimplements car along with new callbacks (eOnVehicleAttached, eOnVehicleDetached) and new game_object actor methods get_attached_vehicle(), attach_vehicle() and detach_vehicle()
//-SCRIPTS

// CORE:
	//#define SPAWN_ANTIFREEZE								// spread spawn of game objects thoughout multiple frames to prevent lags (by alpet)
	#define NON_FATAL_VERIFY								// don't crash game when VERIFY fails
//	#define USE_GSC_MEM_ALLOC								// when this is undefined memory allocation for luajit is handled by luajit allocator
//-CORE

// TWEAKS:
	//#define ACTOR_FEEL_GRENADE							// When undefined it disables the grenade HUD indicator for thrown grenades
//-TWEAKS

// SOUND:
	#define NEW_SOUNDS									// use new sounds. Please enclose any new sound addions with this define
	#define LAYERED_SND_SHOOT								// see comment down below 
//-SOUND

/*LAYERED_SND_SHOOT by Alundaio
	When defined, it will allow you to play a group of sounds from a specified section for snd_shoot.
	You can have as many layers as you want, but you must follow naming convention,

	snd_1_layer
	snd_2_layer
	snd_3_layer
	...

	You can also have different variants for each layer defined,

	snd_1_layer
	snd_1_layer1
	snd_1_layer2
	...

	The correct line settings are standard, (ie. snd_1_layer = sound_path, volume, delay)
	ex.

	Here is an example usage:
	snd_shoot = new_snd_section

	[new_snd_section]
	snd_1_layer = weapons\new_sound_shoot1
	snd_1_layer1 = weapons\new_sound_shoot2
	snd_1_layer2 = weapons\new_sound_shoot3
	snd_1_layer3 = weapons\new_sound_shoot4

	snd_2_layer = weapons\mechanical_noise, 1.0, 0.1

	snd_3_layer = weapons\gunshot_echo, 1.0, 0.8
*/