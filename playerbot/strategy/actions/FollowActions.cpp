#include "botpch.h"
#include "../../playerbot.h"
#include "FollowActions.h"
#include "../../PlayerbotAIConfig.h"
#include "../../ServerFacade.h"
#include "../values/Formations.h"

using namespace ai;


bool FollowAction::Execute(Event event)
{
    Formation* formation = AI_VALUE(Formation*, "formation");
    string target = formation->GetTargetName();
    bool moved = false;
    if (!target.empty())
    {
        moved = Follow(AI_VALUE(Unit*, target));
    }
    else
    {
        WorldLocation loc = formation->GetLocation();
        if (Formation::IsNullLocation(loc) || loc.mapid == -1)
            return false;

        moved = MoveTo(loc.mapid, loc.coord_x, loc.coord_y, loc.coord_z);
    }

    return moved;
}

bool FollowAction::isUseful()
{
    Formation* formation = AI_VALUE(Formation*, "formation");
    float distance = 0;
    string target = formation->GetTargetName();

    Unit* fTarget = NULL;
    if(!target.empty())
        fTarget = AI_VALUE(Unit*, target);
    else
        fTarget = AI_VALUE(Unit*, "master target");

    if (fTarget)
    {
        if (fTarget->IsTaxiFlying())
            return false;

        if (!CanDeadFollow(fTarget))
            return false;

        if (fTarget->GetGUIDLow() == bot->GetGUIDLow())
            return false;        
    }

    if (!target.empty())
    {
        distance = AI_VALUE2(float, "distance", target);
    }
    else
    {
        WorldLocation loc = formation->GetLocation();
        if (Formation::IsNullLocation(loc) || bot->GetMapId() != loc.mapid)
            return false;

        distance = sServerFacade.GetDistance2d(bot, loc.coord_x, loc.coord_y);
    }

    return sServerFacade.IsDistanceGreaterThan(distance, formation->GetMaxDistance());
}

bool FollowAction::CanDeadFollow(Unit* target)
{
    //Follow ghost when dead
    if (!sServerFacade.IsAlive(bot) && !sServerFacade.IsAlive(target) && target->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
        return true;

    //Follow when both alive or guard corpse of master
    if (sServerFacade.IsAlive(bot) && (sServerFacade.IsAlive(target) || !target->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GHOST)))
        return true;

    return false;
}

bool FleeToMasterAction::Execute(Event event)
{
    bool canFollow = Follow(AI_VALUE(Unit*, "master target"));
    if (!canFollow)
    {
        //ai->SetNextCheckDelay(5000);
        return false;
    }
    ai->TellMaster("Wait for me!");
    return true;
}

bool FleeToMasterAction::isUseful()
{
    if (!ai->GetGroupMaster())
        return false;

    if (ai->GetGroupMaster() == bot)
        return false;

    Unit* target = AI_VALUE(Unit*, "current target");

    if (target && ai->GetGroupMaster()->HasTarget(target->GetObjectGuid()))
        return false;

    if (!ai->HasStrategy("follow", BOT_STATE_NON_COMBAT))
        return false;

    Unit* fTarget = AI_VALUE(Unit*, "master target");
    
    if (!CanDeadFollow(fTarget))
        return false;

    return true;
}

