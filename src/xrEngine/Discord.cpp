#include "stdafx.h"
#include "Discord.h"
#include "DiscordRichPresense/discord_register.h"
#include "DiscordRichPresense/discord_rpc.h"
#include "x_ray.h"
#include "IGame_Level.h"

extern BOOL game_value_discord_status;
ENGINE_API xrDiscordPresense g_discord;

void xrDiscordPresense::Initialize()
{
	if (game_value_discord_status)
	{
		DiscordEventHandlers nullHandlers;
		ZeroMemory(&nullHandlers, sizeof(nullHandlers));
        if (!p_engine_flags32.test(ITS_CLEAR_1_4_22))
		    Discord_Initialize("722224673408024587", &nullHandlers, TRUE, nullptr);
        else
            Discord_Initialize("763691093849407508", &nullHandlers, TRUE, nullptr);
		bInitialize = true;
	}
}

void xrDiscordPresense::Shutdown()
{
    if (bInitialize)
    {
        Discord_ClearPresence();
        Discord_Shutdown();
        bInitialize = false;
    }
}

void xrDiscordPresense::SetStatus(StatusId status)
{
    if (!bInitialize)
        return;

    DiscordRichPresence presenseInfo;
    memset(&presenseInfo, 0, sizeof(presenseInfo));

    StatusId realStatus = status;
    ShadersLevelId = 0;

    if (status == StatusId::In_Game)
    {
        if (pApp->Level_Current < pApp->Levels.size())
        {
            CApplication::sLevelInfo& LevelInfo = pApp->Levels[pApp->Level_Current];
            if (LevelInfo.name == nullptr) return;

            if (xr_strcmp(LevelInfo.name, "zaton") == 0)
            {
                ShadersLevelId = 1;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::zaton;
            }

            if (xr_strcmp(LevelInfo.name, "jupiter") == 0)
            {
                ShadersLevelId = 2;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::jupiter;
            }

            if (xr_strcmp(LevelInfo.name, "pripyat") == 0)
            {
                ShadersLevelId = 3;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::pripyat;
            }

            if (xr_strcmp(LevelInfo.name, "k00_marsh") == 0)
            {
                ShadersLevelId = 4;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::k00_marsh;
            }

            if (xr_strcmp(LevelInfo.name, "l01_escape") == 0)
            {
                ShadersLevelId = 5;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l01_escape;
            }

            if (xr_strcmp(LevelInfo.name, "l02_garbage") == 0)
            {
                ShadersLevelId = 6;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l02_garbage;
            }

            if (xr_strcmp(LevelInfo.name, "l03_agroprom") == 0)
            {
                ShadersLevelId = 7;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l03_agroprom;
            }

            if (xr_strcmp(LevelInfo.name, "k01_darkscape") == 0)
            {
                ShadersLevelId = 8;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::k01_darkscape;
            }

            if (xr_strcmp(LevelInfo.name, "l04_darkvalley") == 0)
            {
                ShadersLevelId = 9;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l04_darkvalley;
            }

            if (xr_strcmp(LevelInfo.name, "l05_bar") == 0)
            {
                ShadersLevelId = 10;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l05_bar;
            }

            if (xr_strcmp(LevelInfo.name, "l06_rostok") == 0)
            {
                ShadersLevelId = 11;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l06_rostok;
            }

            if (xr_strcmp(LevelInfo.name, "l07_military") == 0)
            {
                ShadersLevelId = 12;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l07_military;
            }

            if (xr_strcmp(LevelInfo.name, "l08_yantar") == 0)
            {
                ShadersLevelId = 13;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l08_yantar;
            }

            if (xr_strcmp(LevelInfo.name, "l09_deadcity") == 0)
            {
                ShadersLevelId = 14;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l09_deadcity;
            }

            if (xr_strcmp(LevelInfo.name, "l10_limansk") == 0)
            {
                ShadersLevelId = 15;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l10_limansk;
            }

            if (xr_strcmp(LevelInfo.name, "l10_radar") == 0)
            {
                ShadersLevelId = 16;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l10_radar;
            }

            if (xr_strcmp(LevelInfo.name, "l10_red_forest") == 0)
            {
                ShadersLevelId = 17;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l10_red_forest;
            }

            if (xr_strcmp(LevelInfo.name, "l11_pripyat") == 0)
            {
                ShadersLevelId = 18;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l11_pripyat;
            }

            if (xr_strcmp(LevelInfo.name, "l12_stancia") == 0)
            {
                ShadersLevelId = 19;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l12_stancia;
            }

            if (xr_strcmp(LevelInfo.name, "l12_stancia_2") == 0)
            {
                ShadersLevelId = 20;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l12_stancia_2;
            }

            if (xr_strcmp(LevelInfo.name, "l13_generators") == 0)
            {
                ShadersLevelId = 21;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l13_generators;
            }

            if (xr_strcmp(LevelInfo.name, "l03u_agr_underground") == 0)
            {
                ShadersLevelId = 22;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l03u_agr_underground;
            }

            if (xr_strcmp(LevelInfo.name, "l04u_labx18") == 0)
            {
                ShadersLevelId = 23;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l04u_labx18;
            }

            if (xr_strcmp(LevelInfo.name, "l08u_brainlab") == 0)
            {
                ShadersLevelId = 24;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l08u_brainlab;
            }

            if (xr_strcmp(LevelInfo.name, "l12u_sarcofag") == 0)
            {
                ShadersLevelId = 25;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l12u_sarcofag;
            }

            if (xr_strcmp(LevelInfo.name, "l12u_control_monolith") == 0)
            {
                ShadersLevelId = 26;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l12u_control_monolith;
            }

            if (xr_strcmp(LevelInfo.name, "l13u_warlab") == 0)
            {
                ShadersLevelId = 27;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::l13u_warlab;
            }

            if (xr_strcmp(LevelInfo.name, "jupiter_underground") == 0)
            {
                ShadersLevelId = 28;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::jupiter_underground;
            }

            if (xr_strcmp(LevelInfo.name, "labx8") == 0)
            {
                ShadersLevelId = 29;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::labx8;
            }

            if (xr_strcmp(LevelInfo.name, "k02_trucks_cemetery") == 0)
            {
                ShadersLevelId = 30;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::k02_trucks_cemetery;
            }

            if (xr_strcmp(LevelInfo.name, "fake_start") == 0)
            {
                ShadersLevelId = 31;
                Msg("# [Discord]: Level ID: %i", ShadersLevelId);
                realStatus = StatusId::fake_start;
            }
        }
    }

    switch (realStatus)
    {
        case StatusId::In_Game:
            presenseInfo.largeImageKey = "bttr_logo";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            break;
        case StatusId::zaton:
            presenseInfo.largeImageKey = "zaton";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Затон";
            break;

        case StatusId::jupiter:
            presenseInfo.largeImageKey = "jupiter";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Юпитер";
            break;

        case StatusId::pripyat:
            presenseInfo.largeImageKey = "pripyat";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Восточная Припять";
            break;

        case StatusId::k00_marsh:
            presenseInfo.largeImageKey = "boloto";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Болота";
            break;

        case StatusId::l01_escape:
            presenseInfo.largeImageKey = "cordon";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Кордон";
            break;

        case StatusId::l02_garbage:
            presenseInfo.largeImageKey = "garbage";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Свалка";
            break;

        case StatusId::l03_agroprom:
            presenseInfo.largeImageKey = "agroprom";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Агропром";
            break;

        case StatusId::k01_darkscape:
            presenseInfo.largeImageKey = "darkscape";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Темная лощина";
            break;

        case StatusId::l04_darkvalley:
            presenseInfo.largeImageKey = "darkvalley";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Темная долина";
            break;

        case StatusId::l05_bar:
            presenseInfo.largeImageKey = "bar";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Бар 100 рентген";
            break;

        case StatusId::l06_rostok:
            presenseInfo.largeImageKey = "rostok";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Дикая территория";
            break;

        case StatusId::l07_military:
            presenseInfo.largeImageKey = "military";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Армейские склады";
            break;

        case StatusId::l08_yantar:
            presenseInfo.largeImageKey = "yantar";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Янтарь";
            break;

        case StatusId::l09_deadcity:
            presenseInfo.largeImageKey = "deadcity";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Мёртвый город";
            break;

        case StatusId::l10_limansk:
            presenseInfo.largeImageKey = "limansk";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Лиманск";
            break;

        case StatusId::l10_radar:
            presenseInfo.largeImageKey = "radar";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Радар";
            break;

        case StatusId::l10_red_forest:
            presenseInfo.largeImageKey = "red_forest";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Рыжий лес";
            break;

        case StatusId::l11_pripyat:
            presenseInfo.largeImageKey = "pripyat_soc";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Центральная Припять";
            break;

        case StatusId::l12_stancia:
            presenseInfo.largeImageKey = "stancia";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: ЧАЭС";
            break;

        case StatusId::l12_stancia_2:
            presenseInfo.largeImageKey = "stancia2";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: ЧАЭС-2";
            break;

        case StatusId::l13_generators:
            presenseInfo.largeImageKey = "generators";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Генераторы";
            break;

        case StatusId::l03u_agr_underground:
            presenseInfo.largeImageKey = "agr_underground";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Подземелья НИИ Агропром";
            break;

        case StatusId::l04u_labx18:
            presenseInfo.largeImageKey = "labx18";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Лаборатория Х-18";
            break;

        case StatusId::l08u_brainlab:
            presenseInfo.largeImageKey = "brainlab";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Лаборатория Х-10";
            break;

        case StatusId::l12u_sarcofag:
            presenseInfo.largeImageKey = "sarcofag";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Саркофаг";
            break;

        case StatusId::l12u_control_monolith:
            presenseInfo.largeImageKey = "control_monolith";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Управление монолитом";
            break;

        case StatusId::l13u_warlab:
            presenseInfo.largeImageKey = "warlab";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Варлаб";
            break;

        case StatusId::jupiter_underground:
            presenseInfo.largeImageKey = "jupiter_underground2";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Путепровод Припять-1";
            break;

        case StatusId::labx8:
            presenseInfo.largeImageKey = "labx8";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Лаборатория Х-8";
            break;

        case StatusId::k02_trucks_cemetery:
            presenseInfo.largeImageKey = "trucks_cemetery";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.details = "Локация: Кладбище техники";
            break;

        case StatusId::fake_start:
            presenseInfo.largeImageKey = "bttr_logo";
            presenseInfo.state = renderer_value == 0 ? "В игре. DX10 (Static)" : renderer_value == 1 ? "В игре. DX10" :
                renderer_value == 2 ? "В игре. DX11 (Static)" : renderer_value == 3 ? "В игре. DX11" : "В игре. ???";
            presenseInfo.state = "Локация: Фейк старт";
            break;

        case StatusId::Initialization:
            presenseInfo.largeImageKey = "bttr_logo";
            presenseInfo.state = "Загрузка";
            presenseInfo.details = "Инициализация...";
            break;
        case StatusId::Loading_cform:
            presenseInfo.largeImageKey = "bttr_logo";
            presenseInfo.state = "Загрузка геометрии";
            presenseInfo.details = "Инициализация...";
            break;
        case StatusId::Creating_new_game:
            presenseInfo.largeImageKey = "bttr_logo";
            presenseInfo.state = "Создание новой игры";
            presenseInfo.details = "Инициализация...";
            break;

        case StatusId::Loading_alife_simulator:
            presenseInfo.largeImageKey = "bttr_logo";
            presenseInfo.state = "Загрузка A-Life";
            presenseInfo.details = "Инициализация...";
            break;
        case StatusId::Validating_cdkey:
            presenseInfo.largeImageKey = "bttr_logo";
            presenseInfo.state = "Проверка ключей GameSpy";
            presenseInfo.details = "Инициализация...";
            break;

        case StatusId::Loading_ai_objects:
            presenseInfo.largeImageKey = "bttr_logo";
            presenseInfo.state = "Загрузка АИ объектов";
            presenseInfo.details = "Инициализация...";
            break;

        case StatusId::Client_spawning:
            presenseInfo.largeImageKey = "bttr_logo";
            presenseInfo.state = "Загрузка клиента";
            presenseInfo.details = "Инициализация...";
            break;

        case StatusId::Loading_textures:
            presenseInfo.largeImageKey = "bttr_logo";
            presenseInfo.state = "Загрузка текстур";
            presenseInfo.details = "Инициализация...";
            break;

        case StatusId::Client_synchronising:
            presenseInfo.largeImageKey = "bttr_logo";
            presenseInfo.state = "Синхронизация клиента";
            presenseInfo.details = "Инициализация..."; 
            break;

        case StatusId::Server_starting:
            presenseInfo.largeImageKey = "bttr_logo";
            presenseInfo.state = "Старт загрузки функций";
            presenseInfo.details = "Инициализация..."; 
            break;

        case StatusId::Loading_shaders:
            presenseInfo.largeImageKey = "bttr_logo";
            presenseInfo.state = "Загрузка текстур";
            presenseInfo.details = "Инициализация...";
            break;

        case StatusId::Loading_geometry:
            presenseInfo.largeImageKey = "bttr_logo";
            presenseInfo.state = "Загрузка геометрии";
            presenseInfo.details = "Инициализация...";
            break;

        case StatusId::Loading_spatial_db:
            presenseInfo.largeImageKey = "bttr_logo";
            presenseInfo.state = "Загрузка базы рендера";
            presenseInfo.details = "Инициализация...";
            break;

        case StatusId::Loading_details:
            presenseInfo.largeImageKey = "bttr_logo";
            presenseInfo.state = "Загрузка базы рендера";
            presenseInfo.details = "Инициализация...";
            break;
        case StatusId::Loading_lights:
            presenseInfo.largeImageKey = "bttr_logo";
            presenseInfo.state = "Загрузка света локации";
            presenseInfo.details = "Инициализация...";
            break;

        case StatusId::Menu:

        default:
            presenseInfo.details = "В главном меню";
            presenseInfo.largeImageKey = "bttr_logo";
            break;
    }

    presenseInfo.smallImageKey = "ico_bttr";
    presenseInfo.startTimestamp = time(0);

    if (!p_engine_flags32.test(ITS_CLEAR_1_4_22))
        presenseInfo.smallImageText = "S.T.A.L.K.E.R.: Back to the Roots";
    else
        presenseInfo.smallImageText = "S.T.A.L.K.E.R.: Call of Chernobyl";

    presenseInfo.largeImageText = "CryRay Engine";

    Discord_UpdatePresence(&presenseInfo);
}

xrDiscordPresense::~xrDiscordPresense() { Shutdown(); }
