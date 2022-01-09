#include "botpch.h"
#include "../../playerbot.h"
#include "TankWithoutAuraValue.h"

#include "../../ServerFacade.h"
using namespace ai;

extern vector<string> split(const string &s, char delim);

class TankWithoutAuraPredicate : public FindPlayerPredicate, public PlayerbotAIAware
{
    bool onlyMainTank = false;
public:
    TankWithoutAuraPredicate(PlayerbotAI* ai, string aura, bool onlyMainTank) :
        onlyMainTank(onlyMainTank), PlayerbotAIAware(ai), FindPlayerPredicate(), auras(split(aura, ',')) {}

public:
    virtual bool Check(Unit* unit)
    {
        if (!sServerFacade.IsAlive(unit)) return false;
        
        if (!ai->IsTank((Player*)unit))
            return false;

        Group* group = ai->GetBot()->GetGroup();
        
        if (group && group->IsRaidGroup() && onlyMainTank && !group->IsAssistant(unit->GetObjectGuid()))
            return false;

        for (vector<string>::iterator i = auras.begin(); i != auras.end(); ++i)
        {
            if (ai->HasAura(*i, unit))
                return false;
        }

        return true;
    }

private:
    vector<string> auras;
};

Unit* TankWithoutAuraValue::Calculate()
{
    TankWithoutAuraPredicate predicate(ai, qualifier, onlyMainTank);
    return FindPartyMember(predicate);
}
