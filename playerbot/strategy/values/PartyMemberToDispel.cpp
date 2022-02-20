#include "botpch.h"
#include "../../playerbot.h"
#include "PartyMemberToDispel.h"

#include "../../ServerFacade.h"
#include <playerbot/strategy/values/Stances.h>
using namespace ai;

class PartyMemberToDispelPredicate : public FindPlayerPredicate, public PlayerbotAIAware
{
public:
    PartyMemberToDispelPredicate(PlayerbotAI* ai, uint32 dispelType) :
        PlayerbotAIAware(ai), FindPlayerPredicate(), dispelType(dispelType) {}

public:
    virtual bool Check(Unit* unit)
    {
        Pet* pet = dynamic_cast<Pet*>(unit);
        if (pet && (pet->getPetType() == MINI_PET || pet->getPetType() == SUMMON_PET))                    
            return false;

        Stance* stance = ai->GetAiObjectContext()->GetValue<Stance*>("stance")->Get();        

        if (stance && stance->GetName() != "near" &&
            sServerFacade.GetDistance2d(ai->GetBot(), unit) > ai->GetRange("spell"))     
                return false;        

        return sServerFacade.IsAlive(unit) && ai->HasAuraToDispel(unit, dispelType);

    }

private:
    uint32 dispelType;
};

Unit* PartyMemberToDispel::Calculate()
{
    uint32 dispelType = atoi(qualifier.c_str());

    PartyMemberToDispelPredicate predicate(ai, dispelType);
    return FindPartyMember(predicate);
}


bool HasAreaDebuffValue::Calculate()
{
    for (uint32 auraType = SPELL_AURA_BIND_SIGHT; auraType < TOTAL_AURAS; auraType++)
    {
        Unit::AuraList const& auras = ai->GetBot()->GetAurasByType((AuraType)auraType);

        if (auras.empty())
            continue;

        for (Unit::AuraList::const_iterator i = auras.begin(); i != auras.end(); i++)
        {
            Aura* aura = *i;
            if (!aura)
                continue;
            
            SpellEntry const* proto = aura->GetSpellProto();

            if (IsSpellEffectTriggerSpellByAura(proto, aura->GetEffIndex()))
            {
                uint32 trigger_spell_id = proto->EffectTriggerSpell[aura->GetEffIndex()];
                return trigger_spell_id == 29767;//Overload
            }  
            else
            {
                if (!aura->IsPositive() && aura->IsPeriodic())
                {
                    if (proto)
                    {
                        for (int i = 0; i < MAX_EFFECT_INDEX; i++)
                        {
                            SpellRadiusEntry const* radius = sSpellRadiusStore.LookupEntry(proto->EffectRadiusIndex[i]);

                            if (radius)
                                return radius->Radius > 0;
                        }
                    }
                }
            }
        }
    }

    return false;
}