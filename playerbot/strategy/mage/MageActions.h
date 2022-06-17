#pragma once

#include "../../ServerFacade.h"
#include "../actions/GenericActions.h"

namespace ai
{
    BUFF_ACTION(CastFireWardAction, "fire ward");
    BUFF_ACTION(CastFrostWardAction, "frost ward");

    class CastFireballAction : public CastRangeSpellAction
    {
    public:
        CastFireballAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "fireball") {}
    };

    class CastScorchAction : public CastRangeSpellAction
    {
    public:
        CastScorchAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "scorch") {}
    };

    class CastFireBlastAction : public CastRangeSpellAction
    {
    public:
        CastFireBlastAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "fire blast") {}
    };

    class CastArcaneBlastAction : public CastBuffSpellAction
    {
    public:
        CastArcaneBlastAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "arcane blast") {}
        virtual string GetTargetName() { return "current target"; }
    };

    class CastArcaneBarrageAction : public CastRangeSpellAction
    {
    public:
        CastArcaneBarrageAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "arcane barrage") {}
    };

    class CastArcaneMissilesAction : public CastRangeSpellAction
    {
    public:
        CastArcaneMissilesAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "arcane missiles") {}
    };

    class CastPyroblastAction : public CastRangeSpellAction
    {
    public:
        CastPyroblastAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "pyroblast") {}
    };

    class CastFlamestrikeAction : public CastRangeSpellAction
    {
    public:
        CastFlamestrikeAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "flamestrike") {}
    };

    class CastFrostNovaAction : public CastRangeSpellAction
    {
    public:
        CastFrostNovaAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "frost nova") {}
        virtual bool isUseful()
        {
            return sServerFacade.IsDistanceLessOrEqualThan(AI_VALUE2(float, "distance", GetTargetName()), 10.0f);
        }
    };

	class CastFrostboltAction : public CastRangeSpellAction
	{
	public:
		CastFrostboltAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "frostbolt") {}
	};

	class CastBlizzardAction : public CastRangeSpellAction
	{
	public:
		CastBlizzardAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "blizzard") {}
        virtual ActionThreatType getThreatType() { return ACTION_THREAT_AOE; }
	};

	class CastArcaneIntellectAction : public CastBuffSpellAction
    {
	public:
		CastArcaneIntellectAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "arcane intellect") {}
	};

	class CastArcaneIntellectOnPartyAction : public BuffOnPartyAction
    {
	public:
		CastArcaneIntellectOnPartyAction(PlayerbotAI* ai) : BuffOnPartyAction(ai, "arcane intellect") {}
	};

	class CastRemoveCurseAction : public CastCureSpellAction
    {
	public:
		CastRemoveCurseAction(PlayerbotAI* ai) : CastCureSpellAction(ai, "remove curse") {}
	};

	class CastRemoveLesserCurseAction : public CastCureSpellAction
    {
	public:
		CastRemoveLesserCurseAction(PlayerbotAI* ai) : CastCureSpellAction(ai, "remove lesser curse") {}
	};

	class CastIcyVeinsAction : public CastBuffSpellAction
    {
	public:
		CastIcyVeinsAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "icy veins") {}
	};

	class CastCombustionAction : public CastBuffSpellAction
    {
	public:
		CastCombustionAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "combustion") {}
	};

    BEGIN_SPELL_ACTION(CastCounterspellAction, "counterspell")
    END_SPELL_ACTION()

    class CastRemoveCurseOnPartyAction : public CurePartyMemberAction
    {
    public:
        CastRemoveCurseOnPartyAction(PlayerbotAI* ai) : CurePartyMemberAction(ai, "remove curse", DISPEL_CURSE) {}
    };

    class CastRemoveLesserCurseOnPartyAction : public CurePartyMemberAction
    {
    public:
        CastRemoveLesserCurseOnPartyAction(PlayerbotAI* ai) : CurePartyMemberAction(ai, "remove lesser curse", DISPEL_CURSE) {}
    };

	class CastConjureFoodAction : public CastBuffSpellAction
    {
	public:
		CastConjureFoodAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "conjure food") {}
	};

	class CastConjureWaterAction : public CastBuffSpellAction
    {
	public:
		CastConjureWaterAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "conjure water") {}
	};

	class CastIceBlockAction : public CastBuffSpellAction
    {
	public:
		CastIceBlockAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "ice block") {}
	};

    class CastMoltenArmorAction : public CastBuffSpellAction
    {
    public:
        CastMoltenArmorAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "molten armor") {}
    };

    class CastMageArmorAction : public CastBuffSpellAction
    {
    public:
        CastMageArmorAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "mage armor") {}
    };

    class CastIceArmorAction : public CastBuffSpellAction
    {
    public:
        CastIceArmorAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "ice armor") {}
    };

    class CastFrostArmorAction : public CastBuffSpellAction
    {
    public:
        CastFrostArmorAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "frost armor") {}
    };

    class CastPolymorphAction : public CastBuffSpellAction
    {
    public:
        CastPolymorphAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "polymorph") {}
        virtual Value<Unit*>* GetTargetValue();
    };

	class CastSpellstealAction : public CastRangeSpellAction
	{
	public:
		CastSpellstealAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "spellsteal") {}
	};

	class CastLivingBombAction : public CastDebuffSpellAction
	{
	public:
	    CastLivingBombAction(PlayerbotAI* ai) : CastDebuffSpellAction(ai, "living bomb") {}
	};

	class CastDragonsBreathAction : public CastRangeSpellAction
	{
	public:
	    CastDragonsBreathAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "dragon's breath") {}
	};

	class CastBlastWaveAction : public CastRangeSpellAction
	{
	public:
	    CastBlastWaveAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "blast wave") {}
	};

	class CastInvisibilityAction : public CastBuffSpellAction
	{
	public:
	    CastInvisibilityAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "invisibility") {}
	};

	class CastEvocationAction : public CastRangeSpellAction
	{
	public:
	    CastEvocationAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "evocation") {}
	    virtual string GetTargetName() { return "self target"; }
	};

    class CastCounterspellOnEnemyHealerAction : public CastSpellOnEnemyHealerAction
    {
    public:
	    CastCounterspellOnEnemyHealerAction(PlayerbotAI* ai) : CastSpellOnEnemyHealerAction(ai, "counterspell") {}
    };

    class CastArcanePowerAction : public CastBuffSpellAction
    {
    public:
        CastArcanePowerAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "arcane power") {}
    };

    class CastPresenceOfMindAction : public CastBuffSpellAction
    {
    public:
        CastPresenceOfMindAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "presence of mind") {}
    };
}
