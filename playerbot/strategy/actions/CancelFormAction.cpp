#include "botpch.h"
#include "../../playerbot.h"
#include "CancelFormAction.h"


using namespace ai;

bool CancelFormAction::Execute(Event event)
{
    if (bot->IsShapeShifted() && bot->IsInDisallowedMountForm())
    {        
        bot->RemoveSpellsCausingAura(SPELL_AURA_MOD_SHAPESHIFT);

        return true;
    } 

    return false;
}

bool CancelFormAction::isPossible()
{
    return Action::isPossible() && bot->getClass() == CLASS_DRUID;
}
