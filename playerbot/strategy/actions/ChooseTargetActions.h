#pragma once

#include "../Action.h"
#include "AttackAction.h"
#include "../../ServerFacade.h"
#include "../../playerbot.h"
#include "../../TravelMgr.h"
#include "../../LootObjectStack.h"
#include "ChooseRpgTargetAction.h"

namespace ai
{
    class DpsAoeAction : public AttackAction
    {
    public:
        DpsAoeAction(PlayerbotAI* ai) : AttackAction(ai, "dps aoe") {}

        virtual string GetTargetName() { return "dps aoe target"; }
    };

    class DpsAssistAction : public AttackAction
    {
    public:
        DpsAssistAction(PlayerbotAI* ai) : AttackAction(ai, "dps assist") {}

        virtual string GetTargetName() { return "dps target"; }
        virtual bool isUseful()
        {
            // if carry flag, do not start fight
            if (bot->HasAura(23333) || bot->HasAura(23335) || bot->HasAura(34976))
                return false;

            return true;
        }
    };
	
	class ActivateCombatState : public Action
    {
    public:
		ActivateCombatState(PlayerbotAI* ai) : Action(ai, "activate combat state") {}

        virtual bool isUseful() { return true; }

		virtual bool isPossible()
		{
			return ai->GetCurrentState() == BOT_STATE_NON_COMBAT;
		}

		virtual bool Execute(Event event)
		{
			ai->ChangeEngine(BOT_STATE_COMBAT);
			ai->ChangeStrategy("-follow,+stance", BOT_STATE_COMBAT);

			return true;
		}
    };

    class TankAssistAction : public AttackAction
    {
    public:
        TankAssistAction(PlayerbotAI* ai) : AttackAction(ai, "tank assist") {}
        virtual string GetTargetName() { return "tank target"; }
    };

    class AttackAnythingAction : public AttackAction
    {
    private:   
    public:
        AttackAnythingAction(PlayerbotAI* ai) : AttackAction(ai, "attack anything") {}
        virtual string GetTargetName() { return "grind target"; }

        virtual bool isUseful();

        virtual bool isPossible()
        {
            return AttackAction::isPossible() && GetTarget();
        }

        virtual bool Execute(Event event)
        {
            bool result = AttackAction::Execute(event);

            if (result)
            {
                Unit* grindTarget = GetTarget();
                if (grindTarget)
                {
                    const char* grindName = grindTarget->GetName();
                    if (grindName)
                    {
                        context->GetValue<ObjectGuid>("pull target")->Set(grindTarget->GetObjectGuid());
                        ai->StopMoving();
                    }
                }
            }
            return result;
        }
    };

    class AttackLeastHpTargetAction : public AttackAction
    {
    public:
        AttackLeastHpTargetAction(PlayerbotAI* ai) : AttackAction(ai, "attack least hp target") {}
        virtual string GetTargetName() { return "least hp target"; }
    };

    class AttackEnemyPlayerAction : public AttackAction
    {
    public:
        AttackEnemyPlayerAction(PlayerbotAI* ai) : AttackAction(ai, "attack enemy player") {}
        virtual string GetTargetName() { return "enemy player target"; }
        virtual bool isUseful() {

            // if carry flag, do not start fight
            if (bot->HasAura(23333) || bot->HasAura(23335) || bot->HasAura(34976))
                return false;

            return !sPlayerbotAIConfig.IsInPvpProhibitedZone(sServerFacade.GetAreaId(bot));
        }
    };

    class AttackRtiTargetAction : public AttackAction
    {
    public:
        AttackRtiTargetAction(PlayerbotAI* ai) : AttackAction(ai, "attack rti target") {}
        virtual string GetTargetName() { return "rti target"; }
    };

    class AttackEnemyFlagCarrierAction : public AttackAction
    {
    public:
        AttackEnemyFlagCarrierAction(PlayerbotAI* ai) : AttackAction(ai, "attack enemy flag carrier") {}
        virtual string GetTargetName() { return "enemy flag carrier"; }
        virtual bool isUseful() {
            Unit* target = context->GetValue<Unit*>("enemy flag carrier")->Get();
            return target && sServerFacade.IsDistanceLessOrEqualThan(sServerFacade.GetDistance2d(bot, target), 75.0f) && (bot->HasAura(23333) || bot->HasAura(23335) || bot->HasAura(34976));
        }
    };

    class DropTargetAction : public Action
    {
    public:
        DropTargetAction(PlayerbotAI* ai, string name = "drop target") : Action(ai, name) {}

		virtual bool IgnoresCasting() { return true; }

        virtual bool Execute(Event event)
        {
            Unit* target = context->GetValue<Unit*>("current target")->Get();

            if (target && sServerFacade.UnitIsDead(target))
            {
                ObjectGuid guid = target->GetObjectGuid();
                if (guid)
                    context->GetValue<LootObjectStack*>("available loot")->Get()->Add(guid);
            }

            ObjectGuid pullTarget = context->GetValue<ObjectGuid>("pull target")->Get();
            list<ObjectGuid> possible = ai->GetAiObjectContext()->GetValue<list<ObjectGuid> >("possible targets no los")->Get();
            if (pullTarget && find(possible.begin(), possible.end(), pullTarget) == possible.end())
            {
                context->GetValue<ObjectGuid>("pull target")->Set(ObjectGuid());

            }
            context->GetValue<Unit*>("current target")->Set(NULL);
            bot->SetSelectionGuid(ObjectGuid());

            // attack next target if in combat
            uint32 attackers = AI_VALUE(uint8, "attacker count");
            if (attackers > 0)
            {
                Unit* enemy = AI_VALUE(Unit*, "enemy player target");
                if (!enemy)
                {
                    ai->ChangeEngine(BOT_STATE_NON_COMBAT);
                    ai->InterruptSpell();
                    bot->AttackStop();

                    if (ai->HasStrategy("dps assist", BOT_STATE_NON_COMBAT))
                        return ai->DoSpecificAction("dps assist", Event(), true);
                    if (ai->HasStrategy("tank assist", BOT_STATE_NON_COMBAT))
                        return ai->DoSpecificAction("tank assist", Event(), true);
                }
            }

            ai->ChangeEngine(BOT_STATE_NON_COMBAT);
            ai->InterruptSpell();
            bot->AttackStop();
            Pet* pet = bot->GetPet();
            if (pet)
            {
#ifdef MANGOS
                CreatureAI*
#endif
#ifdef CMANGOS
                    UnitAI*
#endif
                    creatureAI = ((Creature*)pet)->AI();
                if (creatureAI)
                {
#ifdef CMANGOS
                    creatureAI->SetReactState(REACT_PASSIVE);
#endif
#ifdef MANGOS
                    pet->GetCharmInfo()->SetReactState(REACT_PASSIVE);
                    pet->GetCharmInfo()->SetCommandState(COMMAND_FOLLOW);
#endif
                    pet->AttackStop();
                }
            }
            return true;
        }
    };

	class LeaveCombatState : public DropTargetAction
	{
	public:
		LeaveCombatState(PlayerbotAI* ai) : DropTargetAction(ai, "leave combat state") {}

		virtual bool Execute(Event event)
		{
			ai->ChangeEngine(BOT_STATE_NON_COMBAT);
			ai->ChangeStrategy("-follow", BOT_STATE_COMBAT);
			return DropTargetAction::Execute(event);
		}
	};

}
