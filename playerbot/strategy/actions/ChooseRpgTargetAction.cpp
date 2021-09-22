#include "botpch.h"
#include "../../playerbot.h"
#include "ChooseRpgTargetAction.h"
#include "../../PlayerbotAIConfig.h"
#include "../values/PossibleRpgTargetsValue.h"
#include "../../Travelmgr.h"
#include "../values/Formations.h"
#include "../values/BudgetValues.h"
#include "GuildCreateActions.h"

using namespace ai;

bool ChooseRpgTargetAction::CanTrain(ObjectGuid guid)
{
    Creature* creature = ai->GetCreature(guid);

    if (!creature)
        return false;

    if (!creature->IsTrainerOf(bot, false))
        return false;

    // check present spell in trainer spell list
    TrainerSpellData const* cSpells = creature->GetTrainerSpells();
    TrainerSpellData const* tSpells = creature->GetTrainerTemplateSpells();
    if (!cSpells && !tSpells)
    {
        return false;
    }

    float fDiscountMod = bot->GetReputationPriceDiscount(creature);

    TrainerSpellMap trainer_spells;      
    if(cSpells)
        trainer_spells.insert(cSpells->spellList.begin(), cSpells->spellList.end());
    if(tSpells)
        trainer_spells.insert(tSpells->spellList.begin(), tSpells->spellList.end());

    for (TrainerSpellMap::const_iterator itr = trainer_spells.begin(); itr != trainer_spells.end(); ++itr)
    {
        TrainerSpell const* tSpell = &itr->second;

        if (!tSpell)
            continue;

        uint32 reqLevel = 0;

        reqLevel = tSpell->isProvidedReqLevel ? tSpell->reqLevel : std::max(reqLevel, tSpell->reqLevel);
        TrainerSpellState state = bot->GetTrainerSpellState(tSpell, reqLevel);
        if (state != TRAINER_SPELL_GREEN)
            continue;

        uint32 spellId = tSpell->spell;
        const SpellEntry* const pSpellInfo = sServerFacade.LookupSpellInfo(spellId);
        if (!pSpellInfo)
            continue;

        uint32 cost = uint32(floor(tSpell->spellCost * fDiscountMod));
        if (cost > AI_VALUE2(uint32, "free money for", (uint32)NeedMoneyFor::spells))
            continue;

        return true;
    }
    return false;
}

BattleGroundTypeId ChooseRpgTargetAction::CanQueueBg(ObjectGuid guid)
{
    for (uint32 i = 1; i < MAX_BATTLEGROUND_QUEUE_TYPES; i++)
    {
        BattleGroundQueueTypeId queueTypeId = (BattleGroundQueueTypeId)i;

        BattleGroundTypeId bgTypeId = sServerFacade.BgTemplateId(queueTypeId);

        BattleGround* bg = sBattleGroundMgr.GetBattleGroundTemplate(bgTypeId);
        if (!bg)
            continue;

        if (bot->getLevel() < bg->GetMinLevel())
            continue;

        // check if already in queue
        if (bot->InBattleGroundQueueForBattleGroundQueueType(queueTypeId))
            continue;        

        map<Team, map<BattleGroundTypeId, list<uint32>>> battleMastersCache = sRandomPlayerbotMgr.getBattleMastersCache();

        for (auto& entry : battleMastersCache[TEAM_BOTH_ALLOWED][bgTypeId])
            if (entry == guid.GetEntry())
                return bgTypeId;

        for (auto& entry : battleMastersCache[bot->GetTeam()][bgTypeId])
            if (entry == guid.GetEntry())
                return bgTypeId;
    }

    return BATTLEGROUND_TYPE_NONE;
}


bool ChooseRpgTargetAction::CanDiscover(ObjectGuid guid)
{
    Unit* unit = ai->GetUnit(guid);

    if (!unit)
        return false;

    if (!unit->isTaxi())
        return false;

    if (bot->isTaxiCheater())
        return false;

    uint32 node = sObjectMgr.GetNearestTaxiNode(unit->GetPositionX(), unit->GetPositionY(), unit->GetPositionZ(), unit->GetMapId(), bot->GetTeam());

    if (bot->m_taxi.IsTaximaskNodeKnown(node))
        return false;

    return true;
}


uint32 ChooseRpgTargetAction::HasSameTarget(ObjectGuid guid)
{
    if (ai->HasRealPlayerMaster())
        return 0;

    uint32 num = 0;

    list<ObjectGuid> nearGuids = ai->GetAiObjectContext()->GetValue<list<ObjectGuid> >("nearest friendly players")->Get();
    for (auto& i : nearGuids)
    {
        Player* player = sObjectMgr.GetPlayer(i);

        if (!player)
            continue;

        PlayerbotAI* ai = player->GetPlayerbotAI();

        if (!ai)
            continue;

        if (!ai->AllowActivity(GRIND_ACTIVITY))
            continue;

        if (ai->GetAiObjectContext()->GetValue<ObjectGuid>("rpg target")->Get() != guid)
            continue;

        num++;
    }

    return num;
}

bool ChooseRpgTargetAction::Execute(Event event)
{
    TravelTarget* travelTarget = context->GetValue<TravelTarget*>("travel target")->Get();
    list<ObjectGuid> possibleTargets = AI_VALUE(list<ObjectGuid>, "possible rpg targets");
    list<ObjectGuid> possibleObjects = AI_VALUE(list<ObjectGuid>, "nearest game objects no los");
    set<ObjectGuid>& ignoreList = context->GetValue<set<ObjectGuid>&>("ignore rpg target")->Get();

    if (!possibleObjects.empty())
    {
        possibleTargets.insert(possibleTargets.end(), possibleObjects.begin(), possibleObjects.end());
    }

    if (possibleTargets.empty())
    {
        return false;
    }

    vector<ObjectGuid*> targets;

    int maxPriority = 1;

    //First handing in quests
    for (auto& guid : possibleTargets)
    {
        GameObject* go = ai->GetGameObject(guid);
        Unit* unit = ai->GetUnit(guid);

        if (!go && !unit)
            continue;

        int32 entry;

        if (unit)
            entry = unit->GetEntry();
        else
            entry = -((int32)go->GetEntry());

        if (!ignoreList.empty()
            && ignoreList.find(guid) != ignoreList.end()
            && urand(0, 100) < 10) //10% chance to retry ignored.            
            continue;

        int priority = 1;

        if (unit)
        {
            if (!isFollowValid(bot, unit))
                continue;

            if (unit->isTaxi() && !bot->isTaxiCheater())
            {
                uint32 node = sObjectMgr.GetNearestTaxiNode(unit->GetPositionX(), unit->GetPositionY(), unit->GetPositionZ(), unit->GetMapId(), bot->GetTeam());

                if (!bot->m_taxi.IsTaximaskNodeKnown(node))
                    priority = 110;
            }

            if (unit->isVendor() && AI_VALUE2(bool, "group or", "should sell,can sell,following party,near leader"))
                priority = 100;
            else if (unit->isArmorer() && AI_VALUE2(bool, "group or", "should repair,can repair,following party,near leader"))
                priority = 95;
            else if (AI_VALUE2(bool, "can turn in quest npc", unit->GetEntry()))
                priority = 90;
            else if (CanTrain(guid) || AI_VALUE2(bool, "can accept quest npc", unit->GetEntry()) || (!AI_VALUE(bool, "can fight equal") && AI_VALUE2(bool, "can accept quest low level npc", unit->GetEntry())))
                priority = 80;
            else if (travelTarget->getDestination() && travelTarget->getDestination()->getEntry() == unit->GetEntry())
                priority = 70;
            else if (unit->isInnkeeper() && AI_VALUE(bool, "should home bind"))
                priority = 60;
            else if (unit->isBattleMaster() && CanQueueBg(guid) != BATTLEGROUND_TYPE_NONE)
                priority = 50;
            else if (unit->isGuildMaster() && BuyPetitionAction::canBuyPetition(bot))
                priority = 40;
        }
        else
        {
            if (!sServerFacade.isSpawned(go)
                || go->IsInUse()
                || go->GetGoState() != GO_STATE_READY)
                continue;

            if (!isFollowValid(bot, go))
                continue;

            if(AI_VALUE2(bool, "can turn in quest npc", entry))
                priority = 90;
            else if (AI_VALUE2(bool, "can accept quest npc", entry))
                priority = 80;
            else if (travelTarget->getDestination() && travelTarget->getDestination()->getEntry() == entry)
                priority = 70;     
        }

        if (ai->HasStrategy("debug rpg", BOT_STATE_NON_COMBAT) && priority > 1)
        {
            ostringstream out;
            out << "rpg option: ";
            if (unit)
                out << chat->formatWorldobject(unit);
            if (go)
                out << chat->formatGameobject(go);

            out << " " << priority;

            ai->TellMasterNoFacing(out);
        }

        if (priority < maxPriority)
            continue;

        if (HasSameTarget(guid) > urand(5, 15))
            continue;

        if (priority > maxPriority)
            targets.clear();

        maxPriority = priority;

        targets.push_back(&guid);
    }

    if (targets.empty())
    {
        sLog.outDetail("%s can't choose RPG target: all %zu are not available", bot->GetName(), possibleTargets.size());
        ignoreList.clear(); //Clear ignore list.
        context->GetValue<set<ObjectGuid>&>("ignore rpg target")->Set(ignoreList);
        context->GetValue<ObjectGuid>("rpg target")->Set(ObjectGuid());
        return false;
    }

    ObjectGuid* guid = targets[urand(0, targets.size() - 1)];
    if (!guid)
    {
        context->GetValue<ObjectGuid>("rpg target")->Set(ObjectGuid());
        return false;
    }


    GameObject* go = ai->GetGameObject(*guid);
    Unit* unit = ai->GetUnit(*guid);

    if (ai->HasStrategy("debug", BOT_STATE_NON_COMBAT))
    {
        ostringstream out;
        out << "found: ";
        if (unit)
            out << chat->formatWorldobject(unit);
        if (go)
            out << chat->formatGameobject(go);

        out << " " << maxPriority;

        ai->TellMasterNoFacing(out);
    }

    context->GetValue<ObjectGuid>("rpg target")->Set(*guid);

    return true;
}

bool ChooseRpgTargetAction::isUseful()
{
    return ai->AllowActivity(RPG_ACTIVITY)
        && !bot->IsInCombat()
        && !context->GetValue<ObjectGuid>("rpg target")->Get()
        && (!context->GetValue<TravelTarget*>("travel target")->Get()->isTraveling() || !ChooseRpgTargetAction::isFollowValid(bot, context->GetValue<TravelTarget*>("travel target")->Get()->getLocation()))
        && !context->GetValue <list<ObjectGuid>>("possible rpg targets")->Get().empty();
}

bool ChooseRpgTargetAction::isFollowValid(Player* bot, WorldObject* target)
{
    if (!target)
        return false;

    WorldLocation location;
    target->GetPosition(location);
    return isFollowValid(bot, location);
}

bool ChooseRpgTargetAction::isFollowValid(Player* bot, WorldLocation location)
{
    PlayerbotAI* ai = bot->GetPlayerbotAI();
    Player* master = ai->GetGroupMaster();
    AiObjectContext* context = ai->GetAiObjectContext();

    if (!master || bot == master)
        return true;

    if (!ai->HasStrategy("follow", BOT_STATE_NON_COMBAT))
        return true;

    if (bot->GetDistance(master) > sPlayerbotAIConfig.rpgDistance * 2)
        return false;

    Formation* formation = AI_VALUE(Formation*, "formation");
    float distance = master->GetDistance2d(location.coord_x, location.coord_y);

    if (!ai->HasActivePlayerMaster() && distance < 50.0f)
        return true;

    if (!master->IsMoving() && distance < 25.0f)
        return true;

    if (distance < formation->GetMaxDistance())
        return true;

    return false;
}