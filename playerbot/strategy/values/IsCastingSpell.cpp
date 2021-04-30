#include "botpch.h"
#include "IsCastingSpell.h"
#include "../../playerbot.h"
#include "../../PlayerbotAIConfig.h"
#include "../../ServerFacade.h"

using namespace ai;

bool IsCastingSpell::Calculate()
{
    Player* bot = ai->GetBot();
    for (int type = CURRENT_FIRST_NON_MELEE_SPELL; type < CURRENT_MAX_SPELL; ++type)
    {
        Spell* spell = bot->GetCurrentSpell((CurrentSpellTypes)type);
        if (spell && spell->getState() != SPELL_STATE_FINISHED) 
            return true;
    }

    return false;
}

