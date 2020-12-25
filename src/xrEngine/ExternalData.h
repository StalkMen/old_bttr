#pragma once

// OldSerpskiStalker
namespace DATARENDER
{
	class ExteranData
	{
	public:
		// Фикс дофа в прицеле, конфликтует с погодным дофом
		bool					is_zoomed;

		void					ZoomActive(bool reload_only)
		{
			is_zoomed = reload_only;
		};

		bool					IsZoomActive()
		{
			return is_zoomed;
		};

		// Подобная функция, сейчас мы перезаряжаемся
		bool					is_reload;

		void					ReloadActive(bool reload_only)
		{
			is_reload = reload_only;
		};

		bool					IsReloadActive()
		{
			return is_reload;
		};

		// Есть ли стекло в головном уборе ?
		bool					is_helmet_with_glass;

		void					HelmetWithGlassActive(bool reload_only)
		{
			is_helmet_with_glass = reload_only;
		};

		bool					IsHelmetWithGlassActive()
		{
			return is_helmet_with_glass;
		};

		// Есть ли встроенная маска в броне ?
		bool					is_outfit_with_glass;

		void					OutfitWithGlassActive(bool reload_only)
		{
			is_outfit_with_glass = reload_only;
		};

		bool					IsOutfitWithGlassActive()
		{
			return is_outfit_with_glass;
		};

		// Состояние шлема
		float					helmet_condition;

		void					HelmetCondition(float reload_only)
		{
			helmet_condition = reload_only;
		};

		float					HelmetConditionActive()
		{
			return helmet_condition;
		};

		// Состояние брони
		float					outfit_condition;

		void					OutfitCondition(float reload_only)
		{
			outfit_condition = reload_only;
		};

		float					OutfitConditionActive()
		{
			return outfit_condition;
		};

		// Предпреждение для опций
		bool					restart_level;

		void					RestartLevel(bool reload_only)
		{
			restart_level = reload_only;
		};

		bool					IsRestartLevel()
		{
			return restart_level;
		};
	};
}