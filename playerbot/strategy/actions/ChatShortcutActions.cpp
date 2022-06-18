#include "botpch.h"
#include "../../playerbot.h"
#include "ChatShortcutActions.h"
#include "../../PlayerbotAIConfig.h"
#include "../values/PositionValue.h"
#include "../values/Formations.h"

using namespace ai;

void ReturnPositionResetAction::ResetReturnPosition()
{
    ai::PositionMap& posMap = context->GetValue<ai::PositionMap&>("position")->Get();
    ai::PositionEntry pos = posMap["return"];
    pos.Reset();
    posMap["return"] = pos;
}

void ReturnPositionResetAction::SetReturnPosition(float x, float y, float z)
{
    ai::PositionMap& posMap = context->GetValue<ai::PositionMap&>("position")->Get();
    ai::PositionEntry pos = posMap["return"];
    pos.Set(x, y, z, ai->GetBot()->GetMapId());
    posMap["return"] = pos;
}

bool FollowChatShortcutAction::Execute(Event event)
{
    Player* master = GetMaster();
    if (!master)
        return false;

    ai->Reset();
    ai->ChangeStrategy("+follow,-passive", BOT_STATE_NON_COMBAT);
    ai->ChangeStrategy("-follow,-passive", BOT_STATE_COMBAT);

    ai->TellMaster("Following");
    return true;
}

bool StayChatShortcutAction::Execute(Event event)
{
    Player* master = GetMaster();
    if (!master)
        return false;

    SetReturnPosition(bot->GetPositionX(), bot->GetPositionY(), bot->GetPositionZ());

	ai->Reset();
	ai->ChangeStrategy("-follow");

    ai->TellMaster("Staying");

    return true;
}

bool FleeChatShortcutAction::Execute(Event event)
{
    Player* master = GetMaster();
    if (!master)
        return false;

    ai->Reset();
    ai->ChangeStrategy("+follow,+passive", BOT_STATE_NON_COMBAT);
    ai->ChangeStrategy("+follow,+passive", BOT_STATE_COMBAT);
    ResetReturnPosition();
    if (bot->GetMapId() != master->GetMapId() || bot->GetDistance(master) > sPlayerbotAIConfig.sightDistance)
    {
        ai->TellError("I will not flee with you - too far away");
        return true;
    }
    ai->TellMaster("Fleeing");
    return true;
}

bool GoawayChatShortcutAction::Execute(Event event)
{
    Player* master = GetMaster();
    if (!master)
        return false;

    ai->Reset();
    ai->ChangeStrategy("+runaway", BOT_STATE_NON_COMBAT);
    ai->ChangeStrategy("+runaway", BOT_STATE_COMBAT);
    ResetReturnPosition();
    ai->TellMaster("Running away");
    return true;
}

bool GrindChatShortcutAction::Execute(Event event)
{
    Player* master = GetMaster();
    if (!master)
        return false;

    ai->Reset();
    ai->ChangeStrategy("+grind,-passive", BOT_STATE_NON_COMBAT);
    ResetReturnPosition();
    ai->TellMaster("Grinding");
    return true;
}

bool TankAttackChatShortcutAction::Execute(Event event)
{
    Player* master = GetMaster();
    if (!master)
        return false;

    if (!ai->IsTank(bot))
        return false;

    ai->Reset();
    ai->ChangeStrategy("-passive", BOT_STATE_NON_COMBAT);
    ai->ChangeStrategy("-passive", BOT_STATE_COMBAT);
    ResetReturnPosition();
    ai->TellMaster("Attacking");
    return true;
}

bool MaxDpsChatShortcutAction::Execute(Event event)
{
    Player* master = GetMaster();
    if (!master)
        return false;

    if (!ai->ContainsStrategy(STRATEGY_TYPE_DPS))
        return false;

    ai->Reset();
    ai->ChangeStrategy("-threat,-conserve mana,-cast time,+dps debuff,+boost", BOT_STATE_COMBAT);
    ai->TellMaster("Max DPS!");
    return true;
}
