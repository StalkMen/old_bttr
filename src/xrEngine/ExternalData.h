#pragma once

// OldSerpskiStalker
class ExteranData
{
public:
	// Фикс дофа в прицеле, конфликтует с погодным дофом
	bool					is_zoomed;

	void					ZoomActive(bool zoom_only) 
	{ 
		is_zoomed = zoom_only; 
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
};