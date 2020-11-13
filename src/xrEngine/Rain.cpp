#include "stdafx.h"
#pragma once

#include "Rain.h"
#include "igame_persistent.h"
#include "environment.h"

#include "render.h"
#include "igame_level.h"
#include "../xrcdb/xr_area.h"
#include "xr_object.h"

// Warning: duplicated in dxRainRender
static const int max_desired_items = 2500;
static const float source_radius = 12.5f;
static const float source_offset = 40.f;
static const float max_distance = source_offset*1.25f;
static const float sink_offset = -(max_distance - source_offset);
static const float drop_length = 5.f;
static const float drop_width = 0.30f;
static const float drop_angle = 3.0f;
static const float drop_max_angle = deg2rad(89.f);
static const float drop_max_wind_vel = 100.0f;
static const float drop_speed_min = 40.f;
static const float drop_speed_max = 85.f;

const int max_particles = 1000;
const int particles_cache = 400;
const float particles_time = .3f;

rain_timer_params_new* rain_timers_new = NULL;
Fvector4* rain_params_new = NULL;

CEffect_Rain::CEffect_Rain()
{
	state = stIdle;

	snd_Ambient.create("ambient\\rain", st_Effect, sg_Undefined);
	snd_Wind.create("ambient\\wind", st_Effect, sg_Undefined);

	timerWorld = new RainTimer();
	timerCurrViewEntity = new RainTimer();

	p_create();

	if (!rain_timers_new)
		rain_timers_new = new rain_timer_params_new();
	if (!rain_params_new)
		rain_params_new = new Fvector4();

}

CEffect_Rain::~CEffect_Rain()
{
	snd_Ambient.destroy();
	snd_Wind.destroy();

	p_destroy();

	xr_delete(timerWorld);
	xr_delete(timerCurrViewEntity);

	if (rain_timers_new)
		xr_delete(rain_timers_new);
	if (rain_params_new)
		xr_delete(rain_params_new);

}

float CEffect_Rain::GetWorldWetness() const
{
	float rainTimer = timerWorld->rainTimer;
	float lastRainDuration = timerWorld->lastRainDuration;
	float rainDropTime = timerWorld->rainDropTime;

	float res = ((rainTimer - lastRainDuration) / 20.f + _lerp(0.0f, saturate(lastRainDuration / 20.f), saturate(lastRainDuration)));
	return saturate(res);
}

float CEffect_Rain::GetCurrViewEntityWetness() const
{
	float rainTimer = timerCurrViewEntity->rainTimer;
	float lastRainDuration = timerCurrViewEntity->lastRainDuration;
	float rainDropTime = timerCurrViewEntity->rainDropTime;

	float res = ((rainTimer - lastRainDuration) / rainDropTime + _lerp(0.0f, saturate(lastRainDuration / rainDropTime), saturate(lastRainDuration)));

	if (res > 0)
		return res;
	else
		return 0.f;
}

BOOL rain_timer_params_new::RayPick_new(const Fvector& s, const Fvector& d, float& range, collide::rq_target tgt)
{
	BOOL bRes = TRUE;
	collide::rq_result	RQ;
	CObject* E = g_pGameLevel->CurrentViewEntity();
	bRes = g_pGameLevel->ObjectSpace.RayPick(s, d, range, tgt, RQ, E);
	if (bRes) range = RQ.range;
	return bRes;
}
int rain_timer_params_new::Update_new(BOOL state, bool need_raypick)
{
	float	factor = g_pGamePersistent->Environment().CurrentEnv->rain_density;

	if (factor > EPS_L)
	{
		// is raining	
		if (state)
		{
			// effect is enabled
			Fvector P, D;
			P = Device.vCameraPosition;	// cam position
			D.set(0, 1, 0);				// direction to sky
			float max_dist = max_distance;
			if (!need_raypick || !RayPick_new(P, D, max_dist, collide::rqtBoth))
			{
				// under the sky
				if (!not_first_frame)
				{
					// first frame
					not_first_frame = TRUE;
					rain_drop_time = rain_drop_time_basic / factor;		// speed of getting wet
					rain_timestamp = Device.fTimeGlobal;
					if (rain_timer > EPS)
						rain_timestamp += last_rain_duration - rain_timer - std::min(rain_drop_time, last_rain_duration);
					last_rain_duration = 0;
				}
				// ïðîâåðÿåì, íå îòðèöàòåëåí ëè äîæäåâîé òàéìåð, åñëè îòðèöàòåëåí - îáíóëÿåì
				// òàêîå ìîæåò áûòü ïðè ïåðâîì êàäðå ñ äîæäåì, åñëè äî ýòîãî äîæäü óæå êàê-òî ðàç áûë â òåêóùåé èãðîâîé ñåññèè
				if (rain_timer < 0)
					rain_timer = 0;
				rain_timer = Device.fTimeGlobal - rain_timestamp;
			}
			else
			{
				// under the cover. but may be it just appear
				if (rain_timer > EPS)
				{
					// yes, actor was under the sky recently
					float delta = rain_timer - (Device.fTimeGlobal - previous_frame_time);
					rain_timer = (delta > 0) ? delta : 0;
					if (not_first_frame)
					{
						// first update since rain was stopped
						not_first_frame = FALSE;
						last_rain_duration = Device.fTimeGlobal - rain_timestamp;
					}
				}
			}
		}
		else
		{
			// effect is disabled, reset all
			not_first_frame = FALSE;
			last_rain_duration = 0;
			rain_timer = 0;
			rain_timestamp = Device.fTimeGlobal;
		}
		previous_frame_time = Device.fTimeGlobal;
		timer.set(rain_timer, last_rain_duration, rain_drop_time);
		return IS_RAIN;
	}
	else
	{
		// no rain. but may be it just stop
		if (rain_timer > EPS)
		{
			// yes, it has been raining recently
			// so decrease timer
			float delta = rain_timer - (Device.fTimeGlobal - previous_frame_time);
			rain_timer = (delta > 0) ? delta : 0;
			if (not_first_frame)
			{
				// first update since rain was stopped
				not_first_frame = FALSE;
				last_rain_duration = Device.fTimeGlobal - rain_timestamp;
			}
			previous_frame_time = Device.fTimeGlobal;
		}
		timer.set(rain_timer, last_rain_duration, rain_drop_time);
		return NO_RAIN;
	}
}


void CEffect_Rain::SetTimerNullptr(float rain_timer_value, float last_rain_duration, float rain_drop_time)
{
	timerCurrViewEntity->rainTimer = rain_timer_value;
	timerCurrViewEntity->lastRainDuration = last_rain_duration;
	timerCurrViewEntity->rainDropTime = rain_drop_time;
}

void CEffect_Rain::Born(Item& dest, float radius)
{
	Fvector axis;
	axis.set(0, -1, 0);
	float gust = g_pGamePersistent->Environment().wind_strength_factor;
	float k = g_pGamePersistent->Environment().CurrentEnv->wind_velocity * gust / drop_max_wind_vel;
	clamp(k, 0.f, 1.f);
	float pitch = drop_max_angle * k - PI_DIV_2;
	axis.setHP(g_pGamePersistent->Environment().CurrentEnv->wind_direction, pitch);

	Fvector& view = Device.vCameraPosition;
	float angle = ::Random.randF(0, PI_MUL_2);
	float dist = ::Random.randF();
	dist = _sqrt(dist) * radius;
	float x = dist * _cos(angle);
	float z = dist * _sin(angle);
	dest.D.random_dir(axis, deg2rad(drop_angle));
	dest.P.set(x + view.x - dest.D.x * source_offset, source_offset + view.y, z + view.z - dest.D.z * source_offset);
	dest.fSpeed = ::Random.randF(drop_speed_min, drop_speed_max);

	float height = max_distance;
	RenewItem(dest, height, RayPick(dest.P, dest.D, height, collide::rqtBoth));
}

BOOL CEffect_Rain::RayPick(const Fvector& s, const Fvector& d, float& range, collide::rq_target tgt)
{
	BOOL bRes = TRUE;

	collide::rq_result RQ;
	CObject* E = g_pGameLevel->CurrentViewEntity();
	bRes = g_pGameLevel->ObjectSpace.RayPick(s, d, range, tgt, RQ, E);
	if (bRes)
		range = RQ.range;

	return bRes;
}

void CEffect_Rain::RenewItem(Item& dest, float height, BOOL bHit)
{
	dest.uv_set = Random.randI(2);
	if (bHit)
	{
		dest.dwTime_Life = Device.dwTimeGlobal + iFloor(1000.f * height / dest.fSpeed) - Device.dwTimeDelta;
		dest.dwTime_Hit = Device.dwTimeGlobal + iFloor(1000.f * height / dest.fSpeed) - Device.dwTimeDelta;
		dest.Phit.mad(dest.P, dest.D, height);
	}
	else
	{
		dest.dwTime_Life = Device.dwTimeGlobal + iFloor(1000.f * height / dest.fSpeed) - Device.dwTimeDelta;
		dest.dwTime_Hit = Device.dwTimeGlobal + iFloor(2 * 1000.f * height / dest.fSpeed) - Device.dwTimeDelta;
		dest.Phit.set(dest.P);
	}
}

void CEffect_Rain::SetInvalidateState() { state = stIdle; }
void CEffect_Rain::StopAmbient() { snd_Ambient.stop(); }

void CEffect_Rain::OnFrame()
{
	if (!g_pGameLevel)
		return;

	float factor = g_pGamePersistent->Environment().CurrentEnv->rain_density;
	float wind_volume = g_pGamePersistent->Environment().CurrentEnv->wind_volume;
	static float hemi_factor = 0.0f;
	CObject* pCurrViewEntity = g_pGameLevel->CurrentViewEntity();
	if (pCurrViewEntity && pCurrViewEntity->renderable_ROS())
	{
		float* hemi_cube = pCurrViewEntity->renderable_ROS()->get_luminocity_hemi_cube();
		float hemi_val = std::max(hemi_cube[0], hemi_cube[1]);
		hemi_val = std::max(hemi_val, hemi_cube[2]);
		hemi_val = std::max(hemi_val, hemi_cube[3]);
		hemi_val = std::max(hemi_val, hemi_cube[5]);

		float f = hemi_val;
		float t = Device.fTimeDelta;
		clamp(t, 0.001f, 1.0f);
		hemi_factor = _lerp(hemi_factor, f, t);
	}

	UpdateTimer(*timerWorld, state, false);

	switch (state)
	{
	case stIdle:

	{
		UpdateTimer(*timerCurrViewEntity, state, false);

		if (factor >= EPS_L)
		{
			state = stWorking;
			snd_Ambient.play(nullptr, sm_Looped);
			snd_Ambient.set_position(Fvector().set(0, 0, 0));
			snd_Ambient.set_range(source_offset, source_offset * 2.0f);

			snd_Wind.play(0, sm_Looped);
			snd_Wind.set_position(Fvector().set(0, 0, 0));
			snd_Wind.set_range(source_offset, source_offset * 2.f);

		}
	}
	break;
	case stWorking:
	{
		if (factor < EPS_L)
		{
			state = stIdle;
			snd_Ambient.stop();
			snd_Wind.stop();
			return;
		}
		UpdateTimer(*timerCurrViewEntity, state, true);
	}
	break;
	}

	if (snd_Ambient._feedback())
	{
		snd_Ambient.set_volume(_max(0.1f, factor) * hemi_factor);
	}

	Fvector					sndP;
	// wind sound
	if (snd_Wind._feedback())
	{
		snd_Wind.set_position(sndP);
		snd_Wind.set_volume(wind_volume);
	}

}

void CEffect_Rain::UpdateTimer(RainTimer& timer, States state, bool bNeedRayPick)
{
	float factor = g_pGamePersistent->Environment().CurrentEnv->rain_density;
	if (factor > EPS_L)
	{
		// Is raining
		if (state == States::stWorking)
		{
			// Effect is enabled
			Fvector P, D;
			P.set(Device.vCameraPosition); // Camera position
			D.set(0, 1, 0); // Direction to sky

			float max_dist = max_distance;
			if (!bNeedRayPick || !RayPick(P, D, max_dist, collide::rqtBoth))
			{
				// Under the sky
				if (timer.bFirstRainingFrame)
				{
					// First frame
					timer.bFirstRainingFrame = false;
					timer.rainDropTime = timer.rainDropTimeBasic / factor; // Speed of getting wet
					timer.rainTimestamp = Device.fTimeGlobal;
					if (timer.rainTimer > EPS)
						timer.rainTimestamp += timer.lastRainDuration - timer.rainTimer -
						std::min(timer.rainDropTime, timer.lastRainDuration);

					timer.lastRainDuration = 0.0f;
				}

				if (timer.rainTimer < 0.0f)
					timer.rainTimer = 0.0f;

				timer.rainTimer = Device.fTimeGlobal - timer.rainTimestamp;
			}
			else
			{
				if (timer.rainTimer > EPS)
				{
					float delta = timer.rainTimer - (Device.fTimeGlobal - timer.previousFrameTime);
					timer.rainTimer = (delta > 0.0f) ? delta : 0.0f;
					if (!timer.bFirstRainingFrame)
					{
						timer.bFirstRainingFrame = true;
						timer.lastRainDuration = Device.fTimeGlobal - timer.rainTimestamp;
					}
				}
			}
		}
		else
		{
			timer.bFirstRainingFrame = true;
			timer.lastRainDuration = 0.0f;
			timer.rainTimer = 0.0f;
			timer.rainTimestamp = Device.fTimeGlobal;
		}
		timer.previousFrameTime = Device.fTimeGlobal;
	}
	else
	{
		if (timer.rainTimer > EPS)
		{
			float delta = timer.rainTimer - (Device.fTimeGlobal - timer.previousFrameTime);
			timer.rainTimer = (delta > 0.0f) ? delta : 0.0f;
			if (!timer.bFirstRainingFrame)
			{
				timer.bFirstRainingFrame = true;
				timer.lastRainDuration = Device.fTimeGlobal - timer.rainTimestamp;
			}
			timer.previousFrameTime = Device.fTimeGlobal;
		}
	}
}

void CEffect_Rain::Render()
{
	if (!g_pGameLevel)
		return;

	m_pRender->Render(*this);
}

void CEffect_Rain::Hit(Fvector& pos)
{
	if (0 != ::Random.randI(2))
		return;
	Particle* P = p_allocate();
	if (0 == P)
		return;

	const Fsphere& bv_sphere = m_pRender->GetDropBounds();

	P->time = particles_time;
	P->mXForm.rotateY(::Random.randF(PI_MUL_2));
	P->mXForm.translate_over(pos);
	P->mXForm.transform_tiny(P->bounds.P, bv_sphere.P);
	P->bounds.R = bv_sphere.R;
}

void CEffect_Rain::p_create()
{
	particle_pool.resize(max_particles);
	for (u32 it = 0; it < particle_pool.size(); it++)
	{
		Particle& P = particle_pool[it];
		P.prev = it ? (&particle_pool[it - 1]) : 0;
		P.next = (it < (particle_pool.size() - 1)) ? (&particle_pool[it + 1]) : 0;
	}

	particle_active = 0;
	particle_idle = &particle_pool.front();
}

void CEffect_Rain::p_destroy()
{
	particle_active = 0;
	particle_idle = 0;

	particle_pool.clear();
}

void CEffect_Rain::p_remove(Particle* P, Particle*& LST)
{
	VERIFY(P);
	Particle* prev = P->prev;
	P->prev = NULL;
	Particle* next = P->next;
	P->next = NULL;
	if (prev)
		prev->next = next;
	if (next)
		next->prev = prev;
	if (LST == P)
		LST = next;
}

void CEffect_Rain::p_insert(Particle* P, Particle*& LST)
{
	VERIFY(P);
	P->prev = 0;
	P->next = LST;
	if (LST)
		LST->prev = P;
	LST = P;
}

int CEffect_Rain::p_size(Particle* P)
{
	if (0 == P)
		return 0;
	int cnt = 0;
	while (P)
	{
		P = P->next;
		cnt += 1;
	}
	return cnt;
}

CEffect_Rain::Particle* CEffect_Rain::p_allocate()
{
	Particle* P = particle_idle;
	if (0 == P)
		return NULL;
	p_remove(P, particle_idle);
	p_insert(P, particle_active);
	return P;
}

void CEffect_Rain::p_free(Particle* P)
{
	p_remove(P, particle_active);
	p_insert(P, particle_idle);
}