#pragma once
//-' Конфиг для дефайнов в рендерах
	#define DETAIL_RADIUS						// Управление настройки травы
	#define DISABLED_PRECOMPILED_SHADERS_USAGE  // Отключить искользование прекомпилированных шейдеров
	#define FIX_CLEAR_OCC						// Исправление чистки памяти для ОСС
	#define ___DISABLED							// Общее отключение функций для рендера

	// Static geometry optimization
	#define O_S_L1_S_LOW    10.f // geometry 3d volume size
	#define O_S_L1_D_LOW    150.f // distance, after which it is not rendered
	#define O_S_L2_S_LOW    100.f
	#define O_S_L2_D_LOW    200.f
	#define O_S_L3_S_LOW    500.f
	#define O_S_L3_D_LOW    250.f
	#define O_S_L4_S_LOW    2500.f
	#define O_S_L4_D_LOW    350.f
	#define O_S_L5_S_LOW    7000.f
	#define O_S_L5_D_LOW    400.f

	#define O_S_L1_S_MED    25.f
	#define O_S_L1_D_MED    50.f
	#define O_S_L2_S_MED    200.f
	#define O_S_L2_D_MED    150.f
	#define O_S_L3_S_MED    1000.f
	#define O_S_L3_D_MED    200.f
	#define O_S_L4_S_MED    2500.f
	#define O_S_L4_D_MED    300.f
	#define O_S_L5_S_MED    7000.f
	#define O_S_L5_D_MED    400.f

	#define O_S_L1_S_HII    50.f
	#define O_S_L1_D_HII    50.f
	#define O_S_L2_S_HII    400.f
	#define O_S_L2_D_HII    150.f
	#define O_S_L3_S_HII    1500.f
	#define O_S_L3_D_HII    200.f
	#define O_S_L4_S_HII    5000.f
	#define O_S_L4_D_HII    300.f
	#define O_S_L5_S_HII    20000.f
	#define O_S_L5_D_HII    350.f

	#define O_S_L1_S_ULT    50.f
	#define O_S_L1_D_ULT    35.f
	#define O_S_L2_S_ULT    500.f
	#define O_S_L2_D_ULT    125.f
	#define O_S_L3_S_ULT    1750.f
	#define O_S_L3_D_ULT    175.f
	#define O_S_L4_S_ULT    5250.f
	#define O_S_L4_D_ULT    250.f
	#define O_S_L5_S_ULT    25000.f
	#define O_S_L5_D_ULT    300.f 

	// Dyn geometry optimization
	#define O_D_L1_S_LOW    1.f // geometry 3d volume size
	#define O_D_L1_D_LOW    80.f // distance, after which it is not rendered
	#define O_D_L2_S_LOW    3.f
	#define O_D_L2_D_LOW    150.f
	#define O_D_L3_S_LOW    4000.f
	#define O_D_L3_D_LOW    250.f

	#define O_D_L1_S_MED    1.f
	#define O_D_L1_D_MED    40.f
	#define O_D_L2_S_MED    4.f
	#define O_D_L2_D_MED    100.f
	#define O_D_L3_S_MED    4000.f
	#define O_D_L3_D_MED    200.f

	#define O_D_L1_S_HII    1.4f
	#define O_D_L1_D_HII    30.f
	#define O_D_L2_S_HII    4.f
	#define O_D_L2_D_HII    80.f
	#define O_D_L3_S_HII    4000.f
	#define O_D_L3_D_HII    150.f

	#define O_D_L1_S_ULT    2.0f
	#define O_D_L1_D_ULT    30.f
	#define O_D_L2_S_ULT    8.f
	#define O_D_L2_D_ULT    50.f
	#define O_D_L3_S_ULT    4000.f
	#define O_D_L3_D_ULT    110.f

	#define BASE_FOV 67.f //maybe edit it to g_fov? LV