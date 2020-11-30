#include "stdafx.h"
#include "render.h"

IRender_interface::~IRender_interface() {};

// resources
IRender_Light::~IRender_Light()
{
    EnvCryRay.Render->light_destroy(this);
}
IRender_Glow::~IRender_Glow()
{
    EnvCryRay.Render->glow_destroy(this);
}
