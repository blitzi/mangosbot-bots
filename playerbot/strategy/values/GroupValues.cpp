#include "botpch.h"
#include "../../playerbot.h"
#include "GroupValues.h"
#include "../../ServerFacade.h"

using namespace ai;

list<ObjectGuid> GroupMembersValue::Calculate()
{
	list<ObjectGuid> members;

	Group* group = bot->GetGroup();
	if (group)
	{
		for (GroupReference* ref = group->GetFirstMember(); ref; ref = ref->next())
		{
			members.push_back(ref->getSource()->GetObjectGuid());
		}
	}
	else
		members.push_back(bot->GetObjectGuid());

	return members;
}

bool IsFollowingPartyValue::Calculate()
{
    if (ai->GetGroupMaster() == bot)
        return true;

    if (ai->HasStrategy("follow"))
        return true;

    return false;
};

bool IsNearLeaderValue::Calculate()
{
    Player* groupMaster = ai->GetGroupMaster();

    if (groupMaster == bot)
        return true;

    if (!groupMaster)
        return false;

    return sServerFacade.GetDistance2d(bot, ai->GetGroupMaster()) < sPlayerbotAIConfig.sightDistance;
}

bool BoolANDValue::Calculate()
{
    vector<string> values = split(getQualifier(), ',');

    for (auto value : values)
    {
        if(!AI_VALUE(bool, value))
            return false;
    }

    return true;
}

bool BoolORValue::Calculate()
{
    vector<string> values = split(getQualifier(), ',');

    for (auto value : values)
    {
        if (AI_VALUE(bool, value))
            return true;
    }

    return false;
}

bool GroupBoolANDValue::Calculate()
{    
    Group* group = bot->GetGroup();
    if (group)
    {
        for (GroupReference* ref = group->GetFirstMember(); ref; ref = ref->next())
        {            
            Player* member = ref->getSource();

            PlayerbotAI* memberAi = member->GetPlayerbotAI();

            if (!memberAi)
                continue;

            if (member->GetMapId() != bot->GetMapId())
                continue;

            AiObjectContext* memberContext = memberAi->GetAiObjectContext();

            if (!memberContext)
                continue;

            if (!memberContext->GetValue<bool>("and", getQualifier())->Get())
                return false;
        }

        return true;
    }
    else
        return AI_VALUE2(bool, "and", getQualifier());
};

bool GroupBoolORValue::Calculate()
{
    Group* group = bot->GetGroup();
    if (group)
    {
        for (GroupReference* ref = group->GetFirstMember(); ref; ref = ref->next())
        {
            Player* member = ref->getSource();

            PlayerbotAI* memberAi = member->GetPlayerbotAI();

            if (!memberAi)
                continue;

            if (member->GetMapId() != bot->GetMapId())
                continue;

            AiObjectContext* memberContext = memberAi->GetAiObjectContext();

            if (!memberContext)
                continue;

            if (memberContext->GetValue<bool>("and", getQualifier())->Get())
                return true;
        }

        return false;
    }
    else
        return AI_VALUE2(bool, "and", getQualifier());
};


bool GroupReadyValue::Calculate()
{
	bool inDungeon = !WorldPosition(bot).isOverworld();

	for (ObjectGuid guid : AI_VALUE(list<ObjectGuid>, "group members"))
	{
		Player* member = sObjectMgr.GetPlayer(guid);

		if (!member)
			continue;

		if (inDungeon) //In dungeons all following members need to be alive before continueing.
		{
			PlayerbotAI* memberAi = member->GetPlayerbotAI();

			bool isFollowing = memberAi ? memberAi->HasStrategy("follow", BOT_STATE_NON_COMBAT) : true;

			if (!member->IsAlive() && isFollowing)
				return false;
		}

		//We only wait for members that are in range otherwise we might be waiting for bots stuck in dead loops forever.
		if (ai->GetGroupMaster() && sServerFacade.GetDistance2d(member, ai->GetGroupMaster()) > sPlayerbotAIConfig.sightDistance)
			continue;

		if (member->GetHealthPercent() < sPlayerbotAIConfig.almostFullHealth)
			return false;

		if (!member->GetPower(POWER_MANA))
			continue;

		float mana = (static_cast<float> (member->GetPower(POWER_MANA)) / member->GetMaxPower(POWER_MANA)) * 100;

		if (mana < sPlayerbotAIConfig.mediumMana)
			return false;
	}

	return true;
};