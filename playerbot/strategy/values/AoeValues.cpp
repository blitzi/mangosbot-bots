#include "botpch.h"
#include "../../playerbot.h"
#include "AoeValues.h"

#include "../../PlayerbotAIConfig.h"
#include "../../ServerFacade.h"
using namespace ai;

list<ObjectGuid> FindMaxDensity(Player* bot)
{
    list<ObjectGuid> unitGuids = *bot->GetPlayerbotAI()->GetAiObjectContext()->GetValue<list<ObjectGuid> >("possible targets");

    list<Unit*> units;

    for (auto guid : unitGuids)
    {
        Unit* unit = bot->GetPlayerbotAI()->GetUnit(guid);
        if (unit)
            units.push_back(unit);
    }

    map<ObjectGuid, list<ObjectGuid> > groups;
    int maxCount = 0;
    ObjectGuid maxGroup;
    for (auto i : units)
    {
        ObjectGuid gi = i->GetObjectGuid();

        for (auto j : units)
        {
            if (i == j)
                continue;

            ObjectGuid gj = i->GetObjectGuid();
            float d = i->GetDistance2d(j->GetPositionX(), j->GetPositionY(), DIST_CALC_BOUNDING_RADIUS);
            if (sServerFacade.IsDistanceLessOrEqualThan(d, (sPlayerbotAIConfig.aoeRadius * 2)))
                groups[gi].push_back(gj);
        }

        if (maxCount < groups[gi].size())
        {
            maxCount = groups[gi].size();
            maxGroup = gi;
        }
    }

    if (!maxCount)
        return list<ObjectGuid>();

    return groups[maxGroup];
}

WorldLocation AoePositionValue::Calculate()
{
    list<ObjectGuid> group = FindMaxDensity(bot);
    if (group.empty())
        return WorldLocation();

    // Note: don't know where these values come from or even used.
    float x1, y1, x2, y2;
    for (list<ObjectGuid>::iterator i = group.begin(); i != group.end(); ++i)
    {
        Unit* unit = bot->GetPlayerbotAI()->GetUnit(*i);
        if (!unit)
            continue;

        if (i == group.begin() || x1 > unit->GetPositionX())
            x1 = unit->GetPositionX();
        if (i == group.begin() || x2 < unit->GetPositionX())
            x2 = unit->GetPositionX();
        if (i == group.begin() || y1 > unit->GetPositionY())
            y1 = unit->GetPositionY();
        if (i == group.begin() || y2 < unit->GetPositionY())
            y2 = unit->GetPositionY();
    }
    float x = (x1 + x2) / 2;
    float y = (y1 + y2) / 2;
    float z = bot->GetPositionZ() + CONTACT_DISTANCE;;
    bot->UpdateAllowedPositionZ(x, y, z);
    return WorldLocation(bot->GetMapId(), x, y, z, 0);
}

uint8 AoeCountValue::Calculate()
{
    return FindMaxDensity(bot).size() + 1;//+ 1 to also add the target
}
