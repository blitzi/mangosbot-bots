#include "botpch.h"
//#include "../../playerbot.h"
#include "WarlockActions.h"

using namespace ai;

bool UseSoulStone::Execute(Event event)
{
    if (bot->IsMoving())
    {
        MotionMaster& mm = *bot->GetMotionMaster();
        bot->StopMoving();
        mm.Clear();
    }
    bool used = UseItemAction::Execute(event);

    return used;
}