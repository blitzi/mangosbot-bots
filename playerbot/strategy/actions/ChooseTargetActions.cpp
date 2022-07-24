#pragma once

#include "../Action.h"
#include "ChooseTargetActions.h"
#include "MovementGenerator.h"
#include "CreatureAI.h"

bool AttackAnythingAction::isUseful() {

    Unit* target = GetTarget();

    if (!target)
        return false;

    string name = string(target->GetName());

    if (!name.empty() && name.find("Dummy") != std::string::npos) //Target is not a targetdummy
        return false;

    if(!ChooseRpgTargetAction::isFollowValid(bot, target))                               //Do not grind mobs far away from master.
        return false;

    if (AI_VALUE2(float, "health", "self target") <=  sPlayerbotAIConfig.mediumHealth)   //Bot does not has enough health.
        return false;

    if (AI_VALUE2(float, "mana", "self target") && AI_VALUE2(float, "mana", "self target") <= sPlayerbotAIConfig.mediumMana) //Bot has mana and not enough mana.
        return false;

    if (AI_VALUE2(bool, "group or", "should repair,can repair"))
        return false;

    if (AI_VALUE2(bool, "group or", "should sell,can sell"))
        return false;

    if (context->GetValue<ObjectGuid>("rpg target")->Get())
        return false;

    if(context->GetValue<TravelTarget*>("travel target")->Get()->isTraveling() && ChooseRpgTargetAction::isFollowValid(bot, context->GetValue<TravelTarget*>("travel target")->Get()->getLocation())) //Bot is traveling
        return false;

    return true;
}