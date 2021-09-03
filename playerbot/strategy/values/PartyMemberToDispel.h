#pragma once
#include "../Value.h"
#include "PartyMemberValue.h"

namespace ai
{
    class PartyMemberToDispel : public PartyMemberValue, public Qualified
	{
	public:
        PartyMemberToDispel(PlayerbotAI* ai) :
          PartyMemberValue(ai), Qualified() {}

    protected:
        virtual Unit* Calculate();
	};



    class HasAreaDebuffValue : public BoolCalculatedValue, public Qualified
    {
    public:
        HasAreaDebuffValue(PlayerbotAI* ai) : BoolCalculatedValue(ai) {}

        Unit* GetTarget()
        {            
            AiObjectContext* ctx = AiObject::context;

            return ctx->GetValue<Unit*>(qualifier)->Get();
        }
        virtual bool Calculate();
    };
}
