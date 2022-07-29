#pragma once

#include "../Action.h"
#include "../../PlayerbotAIConfig.h"

namespace ai
{	
	class CastSpellAction : public Action
	{
	public:
		CastSpellAction(PlayerbotAI* ai, string spell) : Action(ai, spell)
		{
			this->spell = spell;
		}

		virtual string GetTargetName() { return "current target"; };
		virtual bool Execute(Event event);
		virtual bool isPossible();
		virtual bool isUseful();
		virtual bool IsCast() { return true; }
		virtual ActionThreatType getThreatType() { return ACTION_THREAT_SINGLE; }

		virtual NextAction** getPrerequisites()
		{
			return NULL;
		}

	protected:
		string spell;
	};

    class CastRangeSpellAction : public CastSpellAction
    {
    public:
        CastRangeSpellAction(PlayerbotAI* ai, string spell) : CastSpellAction(ai, spell),
            range(ai->GetRange("spell"))
        {
            this->spell = spell;
        }

        virtual string GetTargetName() { return "current target"; };
        virtual bool isUseful();
        virtual ActionThreatType getThreatType() { return ACTION_THREAT_SINGLE; }

        virtual NextAction** getPrerequisites()
        {
            if (spell == "mount")
                return NULL;
            if (range > ai->GetRange("spell"))
                return NULL;
            else if (range > ATTACK_DISTANCE)
                return NextAction::merge( NextAction::array(0, new NextAction("reach spell"), NULL), Action::getPrerequisites());
            else
                return NextAction::merge( NextAction::array(0, new NextAction("reach melee"), NULL), Action::getPrerequisites());
        }

    protected:
        float range;
    };

    //---------------------------------------------------------------------------------------------------------------------
    class CastAuraSpellAction : public CastRangeSpellAction
    {
    public:
        CastAuraSpellAction(PlayerbotAI* ai, string spell, bool isOwner = false) : CastRangeSpellAction(ai, spell) { this->isOwner = isOwner; }

        virtual bool isUseful();

    protected:
        bool isOwner;
    };

    //---------------------------------------------------------------------------------------------------------------------
    class CastMeleeSpellAction : public CastRangeSpellAction
    {
    public:
        CastMeleeSpellAction(PlayerbotAI* ai, string spell) : CastRangeSpellAction(ai, spell) {
            range = ATTACK_DISTANCE;

            Unit* target = AI_VALUE(Unit*, "current target");
            if (target)
                range = max(5.0f, bot->GetCombinedCombatReach(target, true));
        }
    };

    //---------------------------------------------------------------------------------------------------------------------
    class CastDebuffSpellAction : public CastAuraSpellAction
    {
    public:
        CastDebuffSpellAction(PlayerbotAI* ai, string spell, bool isOwner = false) : CastAuraSpellAction(ai, spell, isOwner) {}
    };

    class CastDebuffSpellOnAttackerAction : public CastAuraSpellAction
    {
    public:
        CastDebuffSpellOnAttackerAction(PlayerbotAI* ai, string spell, bool isOwner = false) : CastAuraSpellAction(ai, spell, isOwner) {}
        Value<Unit*>* GetTargetValue()
        {
            return context->GetValue<Unit*>("attacker without aura", spell);
        }
        virtual string getName() { return spell + " on attacker"; }
        virtual ActionThreatType getThreatType() { return ACTION_THREAT_AOE; }
    };

    class CastBuffSpellAction : public CastAuraSpellAction
    {
    public:
        CastBuffSpellAction(PlayerbotAI* ai, string spell) : CastAuraSpellAction(ai, spell)
        {
            range = ai->GetRange("spell");
        }

        virtual string GetTargetName() { return "self target"; }
    };

    class CastEnchantItemAction : public CastRangeSpellAction
    {
    public:
        CastEnchantItemAction(PlayerbotAI* ai, string spell) : CastRangeSpellAction(ai, spell)
        {
            range = ai->GetRange("spell");
        }

        virtual bool isPossible();
        virtual string GetTargetName() { return "self target"; }
    };

    //---------------------------------------------------------------------------------------------------------------------

    class CastHealingSpellAction : public CastAuraSpellAction
    {
    public:
        CastHealingSpellAction(PlayerbotAI* ai, string spell, uint8 estAmount = 15.0f) : CastAuraSpellAction(ai, spell)
        {
            this->estAmount = estAmount;
            range = ai->GetRange("spell");
        }
        virtual string GetTargetName() { return "self target"; }
        virtual bool isUseful();
        virtual ActionThreatType getThreatType() { return ACTION_THREAT_AOE; }

    protected:
        uint8 estAmount;
    };

    class CastAoeHealSpellAction : public CastHealingSpellAction
    {
    public:
        CastAoeHealSpellAction(PlayerbotAI* ai, string spell, uint8 estAmount = 15.0f) : CastHealingSpellAction(ai, spell, estAmount) {}
        virtual string GetTargetName() { return "party member to heal"; }
        virtual bool isUseful();
    };

    class CancelHealAction : public Action
    {
    public:
        CancelHealAction(PlayerbotAI* ai) : Action(ai) {}
        virtual string GetTargetName() { return "party member to cancel heal"; }
        virtual bool IgnoresCasting() { return true; }
        virtual bool Execute(Event event)
        {
            Unit* target = GetTargetValue()->Get();

            if (target)
            {
                ostringstream o; o << "cancel heal on " << target->GetName();
                ai->TellMaster(o.str());

                ai->GetBot()->CastStop();
            }

            return true; 
        }

        virtual bool isUseful()
        {
            Unit* target = GetTargetValue()->Get();
            return target != NULL;
        }
    };


    class DamageStopAction : public Action
    {
    public:
        DamageStopAction(PlayerbotAI* ai) : Action(ai) {}
        virtual bool IgnoresCasting() { return true; }
        virtual bool Execute(Event event)
        {
            Unit* target = GetTargetValue()->Get();

            if (target)
            {
                ostringstream o; o << "damage stop for " << target->GetName();
                ai->TellMaster(o.str());
                ai->GetBot()->CastStop();

                ai->GetAiObjectContext()->GetValue<Unit*>("current target")->Set(NULL);
                ai->GetAiObjectContext()->GetValue<Unit*>("enemy player target")->Set(NULL);
				ai->GetAiObjectContext()->GetValue<ObjectGuid>("pull target")->Set(ObjectGuid());
                ai->ChangeEngine(BOT_STATE_NON_COMBAT);
                return true;
            }

            return false;
        }

        virtual bool isUseful()
        {
            Unit* target = GetTargetValue()->Get();
            return target;
        }
    };


    class CastCureSpellAction : public CastRangeSpellAction
    {
    public:
        CastCureSpellAction(PlayerbotAI* ai, string spell) : CastRangeSpellAction(ai, spell)
        {
            range = ai->GetRange("spell");
        }

        virtual string GetTargetName() { return "self target"; }
    };

    class PartyMemberActionNameSupport {
    public:
        PartyMemberActionNameSupport(string spell)
        {
            name = string(spell) + " on party";
        }

        virtual string getName() { return name; }

    private:
        string name;
    };

    class HealPartyMemberAction : public CastHealingSpellAction, public PartyMemberActionNameSupport
    {
    public:
        HealPartyMemberAction(PlayerbotAI* ai, string spell, uint8 estAmount = 15.0f) :
            CastHealingSpellAction(ai, spell, estAmount), PartyMemberActionNameSupport(spell) {}

        virtual string GetTargetName() { return "party member to heal"; }
        virtual string getName() { return PartyMemberActionNameSupport::getName(); }
    };

    class ResurrectPartyMemberAction : public CastRangeSpellAction
    {
    public:
        ResurrectPartyMemberAction(PlayerbotAI* ai, string spell) : CastRangeSpellAction(ai, spell) {}

        virtual string GetTargetName() { return "party member to resurrect"; }
    };
    //---------------------------------------------------------------------------------------------------------------------

    class CurePartyMemberAction : public CastRangeSpellAction, public PartyMemberActionNameSupport
    {
    public:
        CurePartyMemberAction(PlayerbotAI* ai, string spell, uint32 dispelType) :
            CastRangeSpellAction(ai, spell), PartyMemberActionNameSupport(spell)
        {
            this->dispelType = dispelType;
        }

        virtual Value<Unit*>* GetTargetValue();
        virtual string getName() { return PartyMemberActionNameSupport::getName(); }

    protected:
        uint32 dispelType;
    };

    //---------------------------------------------------------------------------------------------------------------------

    class BuffOnPartyAction : public CastBuffSpellAction, public PartyMemberActionNameSupport
    {
    public:
        BuffOnPartyAction(PlayerbotAI* ai, string spell) :
            CastBuffSpellAction(ai, spell), PartyMemberActionNameSupport(spell) {}

        virtual Value<Unit*>* GetTargetValue();
        virtual string getName() { return PartyMemberActionNameSupport::getName(); }
    };

	class BuffOnTankAction : public BuffOnPartyAction
	{
	public:
		BuffOnTankAction(PlayerbotAI* ai, string spell) :
			BuffOnPartyAction(ai, spell) {}

		virtual Value<Unit*>* GetTargetValue();
	};

    //---------------------------------------------------------------------------------------------------------------------

    class CastShootAction : public CastRangeSpellAction
    {
    public:
        CastShootAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "shoot")
        {
            Item* const pItem = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED);
            if (pItem)
            {
                spell = "shoot";

                switch (pItem->GetProto()->SubClass)
                {
                case ITEM_SUBCLASS_WEAPON_GUN:
                    spell += " gun";
                    break;
                case ITEM_SUBCLASS_WEAPON_BOW:
                    spell += " bow";
                    break;
                case ITEM_SUBCLASS_WEAPON_CROSSBOW:
                    spell += " crossbow";
                    break;
                }
            }
        }
        virtual ActionThreatType getThreatType() { return ACTION_THREAT_NONE; }
    };

    class CastLifeBloodAction : public CastHealingSpellAction
    {
    public:
        CastLifeBloodAction(PlayerbotAI* ai) : CastHealingSpellAction(ai, "lifeblood") {}
    };

    class CastGiftOfTheNaaruAction : public CastHealingSpellAction
    {
    public:
        CastGiftOfTheNaaruAction(PlayerbotAI* ai) : CastHealingSpellAction(ai, "gift of the naaru") {}
    };

    class CastArcaneTorrentAction : public CastBuffSpellAction
    {
    public:
        CastArcaneTorrentAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "arcane torrent") {}
    };

    class CastManaTapAction : public CastBuffSpellAction
    {
    public:
        CastManaTapAction(PlayerbotAI* ai) : CastBuffSpellAction(ai, "mana tap") {}
    };

    class CastWarStompAction : public CastRangeSpellAction
    {
    public:
        CastWarStompAction(PlayerbotAI* ai) : CastRangeSpellAction(ai, "war stomp") {}
    };

    class CastSpellOnEnemyHealerAction : public CastRangeSpellAction
    {
    public:
        CastSpellOnEnemyHealerAction(PlayerbotAI* ai, string spell) : CastRangeSpellAction(ai, spell) {}
        Value<Unit*>* GetTargetValue()
        {
            return context->GetValue<Unit*>("enemy healer target", spell);
        }
        virtual string getName() { return spell + " on enemy healer"; }
    };

    class CastSnareSpellAction : public CastDebuffSpellAction
    {
    public:
        CastSnareSpellAction(PlayerbotAI* ai, string spell) : CastDebuffSpellAction(ai, spell) {}
        Value<Unit*>* GetTargetValue()
        {
            return context->GetValue<Unit*>("snare target", spell);
        }
        virtual string getName() { return spell + " on snare target"; }
        virtual ActionThreatType getThreatType() { return ACTION_THREAT_NONE; }
    };

    class CastCrowdControlSpellAction : public CastBuffSpellAction
    {
    public:
        CastCrowdControlSpellAction(PlayerbotAI* ai, string spell) : CastBuffSpellAction(ai, spell) {}
        Value<Unit*>* GetTargetValue()
        {
            return context->GetValue<Unit*>("cc target", GetName());
        }
        virtual bool Execute(Event event) { return ai->CastSpell(GetName(), GetTarget()); }
        virtual bool isPossible() { return ai->CanCastSpell(GetName(), GetTarget()); }
        virtual bool isUseful() { return true; }
        virtual ActionThreatType getThreatType() { return ACTION_THREAT_NONE; }
    };

    class CastProtectSpellAction : public CastRangeSpellAction
    {
    public:
        CastProtectSpellAction(PlayerbotAI* ai, string spell) : CastRangeSpellAction(ai, spell) {}
        virtual string GetTargetName() { return "party member to protect"; }
        virtual bool isUseful()
        {
            return GetTarget() && !ai->HasAura(spell, GetTarget());
        }
        virtual ActionThreatType getThreatType() { return ACTION_THREAT_NONE; }
    };
}
