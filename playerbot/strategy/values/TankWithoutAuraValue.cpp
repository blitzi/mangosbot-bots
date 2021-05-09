#include "botpch.h"
#include "../../playerbot.h"
#include "TankWithoutAuraValue.h"

#include "../../ServerFacade.h"
using namespace ai;

extern vector<string> split(const string &s, char delim);

class TankWithoutAuraPredicate : public FindPlayerPredicate, public PlayerbotAIAware
{
public:
    TankWithoutAuraPredicate(PlayerbotAI* ai, string aura) :
        PlayerbotAIAware(ai), FindPlayerPredicate(), auras(split(aura, ',')) {}

public:
    virtual bool Check(Unit* unit)
    {
        if (!sServerFacade.IsAlive(unit)) return false;
        
        if (!ai->IsTank((Player*)unit))
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
    TankWithoutAuraPredicate predicate(ai, qualifier);
    return FindPartyMember(predicate);
}
