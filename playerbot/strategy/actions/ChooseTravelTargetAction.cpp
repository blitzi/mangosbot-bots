#include "botpch.h"
#include "../../playerbot.h"
#include "../../LootObjectStack.h"
#include "ChooseTravelTargetAction.h"
#include "../../PlayerbotAIConfig.h"
#include "../values/ItemUsageValue.h"
#include "../ItemVisitors.h"
#include "../values/ItemCountValue.h"
#include "../values/ItemUsageValue.h"
#include "../values/BudgetValues.h"


using namespace ai;

bool ChooseTravelTargetAction::Execute(Event event)
{
    //Get the current travel target. This target is no longer active.
    TravelTarget * oldTarget = context->GetValue<TravelTarget *>("travel target")->Get();

    //Select a new target to travel to. 
    TravelTarget newTarget = TravelTarget(ai);   
    getNewTarget(&newTarget, oldTarget);

    //If the new target is not active we failed.
    if (!newTarget.isActive())
       return false;    

    setNewTarget(&newTarget, oldTarget);

    return true;
}

//Select a new travel target.
//Currently this selectes mostly based on priority (current quest > new quest).
//This works fine because destinations can be full (max 15 bots per quest giver, max 1 bot per quest mob).
//
//Eventually we want to rewrite this to be more intelligent.
void ChooseTravelTargetAction::getNewTarget(TravelTarget* newTarget, TravelTarget* oldTarget)
{
    bool foundTarget = false;

    //Enpty bags/repair
    if (!foundTarget && (AI_VALUE2(bool, "group or", "should sell,can sell")))
    {
       // foundTarget = SetNpcFlagTarget(newTarget, { UNIT_NPC_FLAG_VENDOR });                       
    }
       
    if (!foundTarget && AI_VALUE2(bool, "group or", "should repair,can repair"))
    {
       // foundTarget = SetNpcFlagTarget(newTarget, { UNIT_NPC_FLAG_REPAIR });
    }     

    if (!foundTarget)
    {
        foundTarget = SetGroupTarget(newTarget);
    }

    if (!foundTarget && !urand(0,2))
    {
        //foundTarget = SetBuyGearTarget(newTarget);
    }

    if(!foundTarget)
        foundTarget = SetGrindTarget(newTarget);                        
             
     if (!foundTarget && ai->HasStrategy("explore", BOT_STATE_NON_COMBAT)) //Explore a unexplored sub-zone.
        foundTarget = SetExploreTarget(newTarget);    
    
    if(!foundTarget)
        foundTarget = SetCurrentTarget(newTarget, oldTarget);  

    if (!foundTarget)
        SetNullTarget(newTarget);
}

void ChooseTravelTargetAction::setNewTarget(TravelTarget* newTarget, TravelTarget* oldTarget)
{
    //If we are heading to a creature/npc clear it from the ignore list. 
    if (oldTarget && oldTarget == newTarget && newTarget->getEntry())
    {
        set<ObjectGuid>& ignoreList = context->GetValue<set<ObjectGuid>&>("ignore rpg target")->Get();

        for (auto& i : ignoreList)
        {
            if (i.GetEntry() == newTarget->getEntry())
            {
                ignoreList.erase(i);
            }
        }

        context->GetValue<set<ObjectGuid>&>("ignore rpg target")->Set(ignoreList);
    }

    //Actually apply the new target to the travel target used by the bot.
    oldTarget->copyTarget(newTarget);

    //If we are idling but have a master. Idle only 10 seconds.
    if (ai->GetMaster() && oldTarget->isActive() && oldTarget->getDestination()->getName() == "NullTravelDestination")
        oldTarget->setExpireIn(10 * IN_MILLISECONDS);
    else if (oldTarget->isForced()) //Make sure travel goes into cooldown after getting to the destination.
        oldTarget->setExpireIn(HOUR * IN_MILLISECONDS);

    //Clear rpg and pull/grind target. We want to travel, not hang around some more.
    context->GetValue<ObjectGuid>("rpg target")->Set(ObjectGuid());
    context->GetValue<ObjectGuid>("pull target")->Set(ObjectGuid());
};

//Tell the master what travel target we are moving towards.
//This should at some point be rewritten to be denser or perhaps logic moved to ->getTitle()
void ChooseTravelTargetAction::ReportTravelTarget(TravelTarget* newTarget, TravelTarget* oldTarget)
{
    TravelDestination* destination = newTarget->getDestination();

    TravelDestination* oldDestination = oldTarget->getDestination();

    ostringstream out;

    if (newTarget->isForced())
        out << "(Forced) ";

    if (destination->getName() == "QuestRelationTravelDestination" || destination->getName() == "QuestObjectiveTravelDestination")
    {
        QuestTravelDestination* QuestDestination = (QuestTravelDestination*)destination;
        Quest const* quest = QuestDestination->GetQuestTemplate();
        WorldPosition botLocation(bot);
        CreatureInfo const* cInfo = NULL;
        GameObjectInfo const* gInfo = NULL;

        if (destination->getEntry() > 0)
            cInfo = ObjectMgr::GetCreatureTemplate(destination->getEntry());
        else
            gInfo = ObjectMgr::GetGameObjectInfo(destination->getEntry() * -1);

        string Sub;

        if (newTarget->isGroupCopy())
            out << "Following group ";
        else if(oldDestination && oldDestination == destination)
            out << "Continuing ";
        else
            out << "Traveling ";

        out << round(newTarget->getDestination()->distanceTo(&botLocation)) << "y";

        out << " for " << chat->formatQuest(quest);

        out << " to " << QuestDestination->getTitle();

        ai->TellMaster(out);
    }
    else if (destination->getName() == "RpgTravelDestination")
    {
        RpgTravelDestination* RpgDestination = (RpgTravelDestination*)destination;

        WorldPosition botLocation(bot);

        if (newTarget->isGroupCopy())
            out << "Following group ";
        else if (oldDestination && oldDestination == destination)
            out << "Continuing ";
        else
            out << "Traveling ";

        out << round(newTarget->getDestination()->distanceTo(&botLocation)) << "y";

        out << " for ";

        if (AI_VALUE2(bool, "group or", "should sell,can sell"))
            out << "selling items";
        else if (AI_VALUE2(bool, "group or", "should repair,can repair"))
            out << "repairing";
        else
            out << "rpg";

        out << " to " << RpgDestination->getTitle();

        ai->TellMaster(out);
    }
    else if (destination->getName() == "ExploreTravelDestination")
    {
        ExploreTravelDestination* ExploreDestination = (ExploreTravelDestination*)destination;

        WorldPosition botLocation(bot);

        if (newTarget->isGroupCopy())
            out << "Following group ";
        else if (oldDestination && oldDestination == destination)
            out << "Continuing ";
        else
            out << "Traveling ";

        out << round(newTarget->getDestination()->distanceTo(&botLocation)) << "y";

        out << " for exploration";

        out << " to " << ExploreDestination->getTitle();

        ai->TellMaster(out);
    }
    else if (destination->getName() == "GrindTravelDestination")
    {
        GrindTravelDestination* GrindDestination = (GrindTravelDestination*)destination;

        WorldPosition botLocation(bot);

        if (newTarget->isGroupCopy())
            out << "Following group ";
        else if (oldDestination && oldDestination == destination)
            out << "Continuing ";
        else
            out << "Traveling ";

        out << round(newTarget->getDestination()->distanceTo(&botLocation)) << "y";

        out << " for grinding money";

        out << " to " << GrindDestination->getTitle();

        ai->TellMaster(out);
    }
    else if (destination->getName() == "BossTravelDestination")
    {
        BossTravelDestination* BossDestination = (BossTravelDestination*)destination;

        WorldPosition botLocation(bot);

        if (newTarget->isGroupCopy())
            out << "Following group ";
        else if (oldDestination && oldDestination == destination)
            out << "Continuing ";
        else
            out << "Traveling ";

        out << round(newTarget->getDestination()->distanceTo(&botLocation)) << "y";

        out << " for good loot";

        out << " to " << BossDestination->getTitle();

        ai->TellMaster(out);
    }
    else if (destination->getName() == "NullTravelDestination")
    {
        if (!oldTarget->getDestination() || oldTarget->getDestination()->getName() != "NullTravelDestination")
        {
            ai->TellMaster("No where to travel. Idling a bit.");
        }
    }
}

bool ChooseTravelTargetAction::getBestDestination(vector<TravelDestination*>* activeDestinations, vector<WorldPosition*>* activePoints)
{
    if (activeDestinations->empty() || activePoints->empty()) //No targets or no points.
        return false;

    WorldPosition botLocation(bot);
    vector<WorldPosition*> availablePoints = sTravelMgr.getNextPoint(&botLocation, *activePoints); //Pick a good point.

    if (availablePoints.empty()) //No points available.
        return false;

    TravelDestination* targetDestination = NULL;

    //Pick the destination that has this point.
    /*for (auto activeTarget : *activeDestinations)
    {
        for (auto point : activeTarget->getPoints())
        {
            if (point == availablePoints.front())
            {
                targetDestination = activeTarget;
                break;
            }
        }
    }*/

    for (auto activeTarget : *activeDestinations) //Pick the destination that has this point.
        if (activeTarget->distanceTo(availablePoints.front()) == 0)
            targetDestination = activeTarget;

    if (!targetDestination)
        return false;

    activeDestinations->clear();
    activePoints->clear();

    activeDestinations->push_back(targetDestination);
    activePoints->push_back(availablePoints.front());

    return true;
}

bool ChooseTravelTargetAction::SetGroupTarget(TravelTarget* target)
{
    vector<TravelDestination*> activeDestinations;
    vector<WorldPosition*> activePoints;

    list<ObjectGuid> groupPlayers;

    Group* group = bot->GetGroup();
    if (group)
    {
        for (GroupReference* ref = group->GetFirstMember(); ref; ref = ref->next())
        {
            if (ref->getSource() != bot)
            {
                if (ref->getSubGroup() != bot->GetSubGroup())
                {
                    groupPlayers.push_back(ref->getSource()->GetObjectGuid());
                }
                else
                {
                    groupPlayers.push_front(ref->getSource()->GetObjectGuid());
                }
            }
        }
    }

    //Find targets of the group.
    for (auto& member : groupPlayers)
    {
        Player* player = sObjectMgr.GetPlayer(member);

        if (!player)
            continue;

        if (!player->GetPlayerbotAI())
            continue;

        if (!player->GetPlayerbotAI()->GetAiObjectContext())
            continue;

        TravelTarget* groupTarget = player->GetPlayerbotAI()->GetAiObjectContext()->GetValue<TravelTarget*>("travel target")->Get();

        if (groupTarget->isGroupCopy())
            continue;

        if (!groupTarget->isActive())
            continue;

        if (!groupTarget->getDestination()->isActive(bot) || groupTarget->getDestination()->getName() == "RpgTravelDestination")
            continue;

        activeDestinations.push_back(groupTarget->getDestination());
        activePoints.push_back(groupTarget->getPosition());
    }

    if (!getBestDestination(&activeDestinations, &activePoints))
        return false;

    target->setTarget(activeDestinations.front(), activePoints.front(), true);

    return target->isActive();
}

bool ChooseTravelTargetAction::SetCurrentTarget(TravelTarget* target, TravelTarget* oldTarget)
{
    TravelDestination* oldDestination = oldTarget->getDestination();

    if (oldTarget->isMaxRetry(false))
        return false;

    if (!oldDestination) //Does this target have a destination?
        return false;

    WorldPosition botLocation(bot);
    vector<WorldPosition*> availablePoints = oldDestination->nextPoint(&botLocation);

    if (availablePoints.empty())
        return false;

    target->setTarget(oldTarget->getDestination(), availablePoints.front());
    target->setStatus(TRAVEL_STATUS_TRAVEL);
    target->setRetry(false, oldTarget->getRetryCount(false) + 1);

    return target->isActive();
}

bool ChooseTravelTargetAction::SetQuestTarget(TravelTarget* target, bool onlyCompleted)
{
    vector<TravelDestination*> activeDestinations;
    vector<WorldPosition*> activePoints;

    QuestStatusMap& questMap = bot->getQuestStatusMap();

    WorldPosition botLocation(bot);

    //Find destinations related to the active quests.
    for (auto& quest : questMap)
    {
        if (quest.second.m_rewarded)
            continue;

        uint32 questId = quest.first;
        QuestStatusData* questStatus = &quest.second;

        if (onlyCompleted && sObjectMgr.GetQuestTemplate(questId) && !bot->CanRewardQuest(sObjectMgr.GetQuestTemplate(questId), false))
            continue;

        vector<TravelDestination*> questDestinations = sTravelMgr.getQuestTravelDestinations(bot, questId, ai->HasRealPlayerMaster(), false, 5000);
        vector< WorldPosition*> questPoints;
        
        for (auto& questDestination : questDestinations)
        {
            vector< WorldPosition*> destinationPoints = questDestination->getPoints();
            if (!destinationPoints.empty())
                questPoints.insert(questPoints.end(), destinationPoints.begin(), destinationPoints.end());
        }

        if (getBestDestination(&questDestinations, &questPoints))
        {
            activeDestinations.push_back(questDestinations.front());
            activePoints.push_back(questPoints.front());
        }       
        
    }

    if (!getBestDestination(&activeDestinations, &activePoints))
        return false;

    target->setTarget(activeDestinations.front(), activePoints.front());

    return target->isActive();
}

bool ChooseTravelTargetAction::SetNewQuestTarget(TravelTarget* target)
{
    vector<TravelDestination*> activeDestinations;
    vector<WorldPosition*> activePoints;

    WorldPosition botLocation(bot);

    //Find quest givers.
    vector<TravelDestination*> TravelDestinations = sTravelMgr.getQuestTravelDestinations(bot, -1, ai->HasRealPlayerMaster());

    activeDestinations.insert(activeDestinations.end(), TravelDestinations.begin(), TravelDestinations.end());

    //Pick one good point per destination.
    for (auto& activeTarget : activeDestinations)
    {
        vector<WorldPosition*> points = activeTarget->nextPoint(&botLocation);
        if (!points.empty())
            activePoints.push_back(points.front());
    }

    if (!getBestDestination(&activeDestinations, &activePoints))
        return false;

    target->setTarget(activeDestinations.front(), activePoints.front());

    return target->isActive();
}

bool ChooseTravelTargetAction::SetRpgTarget(TravelTarget* target)
{
    vector<TravelDestination*> activeDestinations;
    vector<WorldPosition*> activePoints;

    WorldPosition botLocation(bot);

    //Find rpg npcs
    vector<TravelDestination*> TravelDestinations = sTravelMgr.getRpgTravelDestinations(bot, ai->HasRealPlayerMaster());

    activeDestinations.insert(activeDestinations.end(), TravelDestinations.begin(), TravelDestinations.end());

    //Pick one good point per destination.
    for (auto& activeTarget : activeDestinations)
    {
        vector<WorldPosition*> points = activeTarget->nextPoint(&botLocation);
        if (!points.empty())
            activePoints.push_back(points.front());
    }

    if (!getBestDestination(&activeDestinations, &activePoints))
        return false;

    target->setTarget(activeDestinations.front(), activePoints.front());

    return target->isActive();
}

bool ChooseTravelTargetAction::SetGrindTarget(TravelTarget* target)
{
    vector<TravelDestination*> activeDestinations;
    vector<WorldPosition*> activePoints;

    WorldPosition botLocation(bot);

    //Find grind mobs.
    vector<TravelDestination*> TravelDestinations = sTravelMgr.getGrindTravelDestinations(bot, ai->HasRealPlayerMaster());

    activeDestinations.insert(activeDestinations.end(), TravelDestinations.begin(), TravelDestinations.end());

    //Pick one good point per destination.
    for (auto& activeTarget : activeDestinations)
    {
        vector<WorldPosition*> points = activeTarget->nextPoint(&botLocation);
        if (!points.empty())
            activePoints.push_back(points.front());
    }

    if (!getBestDestination(&activeDestinations, &activePoints))
        return false;

    target->setTarget(activeDestinations.front(), activePoints.front());
    target->setForced(true);
    target->setRetry(true);

    return target->isActive();
}

bool ChooseTravelTargetAction::SetBossTarget(TravelTarget* target)
{
    vector<TravelDestination*> activeDestinations;
    vector<WorldPosition*> activePoints;

    WorldPosition botLocation(bot);

    //Find boss mobs.
    vector<TravelDestination*> TravelDestinations = sTravelMgr.getBossTravelDestinations(bot, ai->HasRealPlayerMaster());

    activeDestinations.insert(activeDestinations.end(), TravelDestinations.begin(), TravelDestinations.end());

    //Pick one good point per destination.
    for (auto& activeTarget : activeDestinations)
    {
        vector<WorldPosition*> points = activeTarget->nextPoint(&botLocation);
        if (!points.empty())
            activePoints.push_back(points.front());
    }

    if (!getBestDestination(&activeDestinations, &activePoints))
        return false;

    target->setTarget(activeDestinations.front(), activePoints.front());

    return target->isActive();
}

bool ChooseTravelTargetAction::SetExploreTarget(TravelTarget* target)
{
    vector<TravelDestination*> activeDestinations;
    vector<WorldPosition*> activePoints;

    WorldPosition botLocation(bot);

    //Find quest givers.
    vector<TravelDestination*> TravelDestinations = sTravelMgr.getExploreTravelDestinations(bot, true, true);

    activeDestinations.insert(activeDestinations.end(), TravelDestinations.begin(), TravelDestinations.end());
    /*
    //Pick one good point per destination.
    for (auto& activeTarget : activeDestinations)
    {
        //271 south shore
        //35 booty bay
        //380 The Barrens The Crossroads
        if(((ExploreTravelDestination * )activeTarget)->getAreaId() == 380)
        {
            activePoints.push_back(activeTarget->getPoints(true)[0]);
        }
    }
    */

    if (activePoints.empty())
    {
        TravelDestinations = sTravelMgr.getExploreTravelDestinations(bot, ai->HasRealPlayerMaster());

        for (auto& activeTarget : activeDestinations)
        {
            vector<WorldPosition*> points = activeTarget->nextPoint(&botLocation);
            if (!points.empty())
            {
                activePoints.push_back(points.front());
            }
        }
    }

    if (!getBestDestination(&activeDestinations, &activePoints))
        return false;

    target->setTarget(activeDestinations.front(), activePoints.front());

    return target->isActive();
}

char* strstri(const char* haystack, const char* needle);

bool ChooseTravelTargetAction::SetNpcFlagTarget(TravelTarget* target, vector<NPCFlags> flags, string name, vector<uint32> items)
{
    WorldPosition* botPos = &WorldPosition(bot);

    vector<TravelDestination*> dests;
    vector<TravelDestination*> possibleDest = sTravelMgr.getRpgTravelDestinations(bot, true, true);

    for (auto& d : possibleDest)
    {
        if (!d->getEntry())
            continue;

        CreatureInfo const* cInfo = ObjectMgr::GetCreatureTemplate(d->getEntry());

        if (!cInfo)
            continue;

        bool foundFlag = false;
        for(auto flag : flags)
            if (cInfo->NpcFlags & flag)
            {
                foundFlag = true;
                break;
            }

        if (!foundFlag)
            continue;

        if (!name.empty() && !strstri(cInfo->Name, name.c_str()) && !strstri(cInfo->SubName, name.c_str()))
            continue;

        if (!items.empty())
        {
            bool foundItem = false;
            VendorItemData const* vItems = nullptr;
            VendorItemData const* tItems = nullptr;

            vItems = sObjectMgr.GetNpcVendorItemList(d->getEntry());

//#ifndef MANGOSBOT_ZERO    
            uint32 vendorId = cInfo->VendorTemplateId;
            if (vendorId)
                tItems = sObjectMgr.GetNpcVendorTemplateItemList(vendorId);
//#endif

            for (auto item : items)
            {
                if (vItems && !vItems->Empty())
                for(auto vitem : vItems->m_items) 
                   if (vitem->item == item)
                    {
                        foundItem = true;
                        break;
                    }
                if(tItems && !tItems->Empty())
                for (auto titem : tItems->m_items)
                    if (titem->item == item)
                    {
                        foundItem = true;
                        break;
                    }
            }

            if (!foundItem)
                continue;
        }

        FactionTemplateEntry const* factionEntry = sFactionTemplateStore.LookupEntry(cInfo->Faction);
        ReputationRank reaction = ai->getReaction(factionEntry);

        if (reaction <= REP_NEUTRAL)
            continue;

        dests.push_back(d);
    }

    if (!dests.empty())
    {
        TravelDestination* dest = *std::min_element(dests.begin(), dests.end(), [botPos](TravelDestination* i, TravelDestination* j) {return i->distanceTo(botPos) < j->distanceTo(botPos); });
        vector <WorldPosition*> points = dest->nextPoint(botPos, true);

        if (points.empty())
            return false;

        target->setTarget(dest, points.front());
        target->setForced(true);
        target->setRetry(true);

        return true;
    }

    return false;
}

bool ChooseTravelTargetAction::SetBuyGearTarget(TravelTarget* target)
{
    vector<NPCFlags> flags = { UNIT_NPC_FLAG_VENDOR };

    WorldPosition* botPos = &WorldPosition(bot);

    TravelDestination* bestDest = NULL;
    VendorItem* bestItem = NULL;
    uint32 bestItemLevel = 0;
    float minDist = 200000;

    vector<TravelDestination*> possibleDest = sTravelMgr.getRpgTravelDestinations(bot, true, true);

    for (auto& d : possibleDest)
    {
        if (!d->getEntry())
            continue;

        CreatureInfo const* cInfo = ObjectMgr::GetCreatureTemplate(d->getEntry());

        if (!cInfo)
            continue;

        bool foundFlag = false;
        for (auto flag : flags)
            if (cInfo->NpcFlags & flag)
            {
                foundFlag = true;
                break;
            }

        if (!foundFlag)
            continue;

        FactionTemplateEntry const* factionEntry = sFactionTemplateStore.LookupEntry(cInfo->Faction);
        ReputationRank reaction = ai->getReaction(factionEntry);

        if (reaction <= REP_NEUTRAL)
            continue;

        bool foundItem = false;
        VendorItemList vendorItems;

        uint32 vendorId = cInfo->VendorTemplateId;
        if (vendorId)
        {
            VendorItemData const* vItems = sObjectMgr.GetNpcVendorItemList(d->getEntry());
            VendorItemData const* tItems = sObjectMgr.GetNpcVendorTemplateItemList(vendorId);

            if (tItems)
                vendorItems.insert(vendorItems.begin(), tItems->m_items.begin(), tItems->m_items.end());
            if (vItems)
                vendorItems.insert(vendorItems.begin(), vItems->m_items.begin(), vItems->m_items.end());
        }

        for (auto& tItem : vendorItems)
        {
            ItemUsage usage = AI_VALUE2(ItemUsage, "item usage", tItem->item);
            ItemPrototype const* proto = sObjectMgr.GetItemPrototype(tItem->item);

            uint32 price = proto->BuyPrice;       
            uint32 itemLevel = proto->ItemLevel;

            // reputation discount
            price = uint32(floor(price * bot->GetReputationPriceDiscount(factionEntry)));

            NeedMoneyFor needMoneyFor = NeedMoneyFor::none;

            switch (usage)
            {
            case ITEM_USAGE_REPLACE:
            case ITEM_USAGE_EQUIP:
                needMoneyFor = NeedMoneyFor::gear;
                break;
            }

            if (needMoneyFor == NeedMoneyFor::none)
                continue;

            if (AI_VALUE2(uint32, "free money for", uint32(needMoneyFor)) < price)
                continue;

            if (itemLevel > bestItemLevel)
            {
                bestItem = tItem;
                bestDest = d;
                minDist = d->distanceTo(botPos);
            }
            else
            {
                if (itemLevel == bestItemLevel)
                {
                    float dist = d->distanceTo(botPos);

                    if (dist <= minDist)
                    {
                        bestItem = tItem;
                        bestDest = d;
                        minDist = dist;
                    }
                }
            }
        }       
    }

    if (bestDest)
    {        
        vector <WorldPosition*> points = bestDest->nextPoint(botPos, true);

        if (points.empty())
            return false;

        target->setTarget(bestDest, points.front());
        target->setForced(true);
        target->setRetry(true);

        return true;
    }

    return false;
}



bool ChooseTravelTargetAction::SetNullTarget(TravelTarget* target)
{
    target->setTarget(sTravelMgr.nullTravelDestination, sTravelMgr.nullWorldPosition, true);
    
    return true;
}

vector<string> split(const string& s, char delim);
char* strstri(const char* haystack, const char* needle);

TravelDestination* ChooseTravelTargetAction::FindDestination(Player* bot, string name)
{
    PlayerbotAI* ai = bot->GetPlayerbotAI();

    AiObjectContext* context = ai->GetAiObjectContext();

    vector<TravelDestination*> dests;

    //Zones
    for (auto& d : sTravelMgr.getExploreTravelDestinations(bot, true, true))
    {
        if (strstri(d->getTitle().c_str(), name.c_str()))
            dests.push_back(d);
    }

    //Npcs
    for (auto& d : sTravelMgr.getRpgTravelDestinations(bot, true, true))
    {
        if (strstri(d->getTitle().c_str(), name.c_str()))
            dests.push_back(d);
    }

    //Mobs
    for (auto& d : sTravelMgr.getGrindTravelDestinations(bot, true, true))
    {
        if (strstri(d->getTitle().c_str(), name.c_str()))
            dests.push_back(d);
    }

    //Bosses
    for (auto& d : sTravelMgr.getBossTravelDestinations(bot, true, true))
    {
        if (strstri(d->getTitle().c_str(), name.c_str()))
            dests.push_back(d);
    }

    WorldPosition* botPos = &WorldPosition(bot);

    if (dests.empty())
        return nullptr;

    TravelDestination* dest = *std::min_element(dests.begin(), dests.end(), [botPos](TravelDestination* i, TravelDestination* j) {return i->distanceTo(botPos) < j->distanceTo(botPos); });

    return dest;
};

bool ChooseTravelTargetAction::isUseful()
{ 
    if (context->GetValue<TravelTarget*>("travel target")->Get()->isActive())
        return false;

    if (context->GetValue<LootObject>("loot target")->Get().IsLootPossible(bot))
        return false;

    if (sServerFacade.IsInCombat(bot, true))
        return false;

    if(context->GetValue<ObjectGuid>("rpg target")->Get())
        return false;

    if (AI_VALUE2(bool, "group or", "should repair,can repair"))
        return true;

    if (AI_VALUE2(bool, "group or", "should sell,can sell"))
        return true;


    return !GetTarget();
}

bool ChooseTravelTargetAction::needForQuest(Unit* target)
{
    bool justCheck = (bot->GetObjectGuid() == target->GetObjectGuid());

    QuestStatusMap& questMap = bot->getQuestStatusMap();
    for (auto& quest : questMap)
    {
        const Quest* questTemplate = sObjectMgr.GetQuestTemplate(quest.first);
        if (!questTemplate)
            continue;

        uint32 questId = questTemplate->GetQuestId();

        if (!questId)
            continue;

        QuestStatus status = bot->GetQuestStatus(questId);

        if ((status == QUEST_STATUS_COMPLETE && !bot->GetQuestRewardStatus(questId)))
        {
            if (!justCheck && !target->HasInvolvedQuest(questId))
                continue;

            return true;
        }
        else if (status == QUEST_STATUS_INCOMPLETE)
        {
            QuestStatusData questStatus = quest.second;

            if (questTemplate->GetQuestLevel() > (int)bot->GetLevel())
                continue;

            for (int j = 0; j < QUEST_OBJECTIVES_COUNT; j++)
            {
                int32 entry = questTemplate->ReqCreatureOrGOId[j];

                if (entry && entry > 0)
                {
                    int required = questTemplate->ReqCreatureOrGOCount[j];
                    int available = questStatus.m_creatureOrGOcount[j];

                    if(required && available < required && (target->GetEntry() == entry || justCheck))
                        return true;
                }         

                if (justCheck)
                {
                    int32 itemId = questTemplate->ReqItemId[j];

                    if (itemId && itemId > 0)
                    {
                        int required = questTemplate->ReqItemCount[j];
                        int available = questStatus.m_itemcount[j];

                        if (required && available < required)
                            return true;
                    }
                }
            }

            if (!justCheck)
            {
                CreatureInfo const* data = sObjectMgr.GetCreatureTemplate(target->GetEntry());

                if (data)
                {
                    uint32 lootId = data->LootId;

                    if (lootId)
                    {
                        if (LootTemplates_Creature.HaveQuestLootForPlayer(lootId, bot))
                            return true;
                    }
                }
            }
        }

    }
    return false;
}

bool ChooseTravelTargetAction::needItemForQuest(uint32 itemId, const Quest* questTemplate, const QuestStatusData* questStatus)
{
    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; i++)
    {
        if (questTemplate->ReqItemId[i] != itemId)
            continue;

        int required = questTemplate->ReqItemCount[i];
        int available = questStatus->m_itemcount[i];

        if (!required)
            continue;

        return available < required;
    }

    return false;
}
