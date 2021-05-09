#include "botpch.h"
#include "IsCastingSpell.h"
#include "../../playerbot.h"
#include "../../PlayerbotAIConfig.h"
#include "../../ServerFacade.h"

using namespace ai;

bool IsCastingSpell::Calculate()
{
    return ai->IsCasting();
}