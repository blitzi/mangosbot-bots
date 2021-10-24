#pragma once

#include "../Action.h"
#include "../../PlayerbotAIConfig.h"

namespace ai
{
    class MovementAction : public Action {
    public:
        MovementAction(PlayerbotAI* ai, string name) : Action(ai, name)
        {
            bot = ai->GetBot();
        }

        virtual bool isPossible() { return ai->GetMoveTimer()->Passed() && Action::isPossible();  }

    protected:
        bool ChaseTo(WorldObject *obj, float distance = 0.0f, float angle = 0.0f);
        bool MoveNear(uint32 mapId, float x, float y, float z, float distance = sPlayerbotAIConfig.contactDistance);
        bool MoveTo(uint32 mapId, float x, float y, float z, bool idle = false, bool react = false);
        bool MoveToStance(Unit* target);
        bool MoveNear(WorldObject* target, float distance = sPlayerbotAIConfig.contactDistance);
        bool MoveToLOS(WorldObject* target, bool ranged = false);
        void WaitForReach(float distance);
        float GetFollowAngle();
        bool Follow(Unit* target, float distance = sPlayerbotAIConfig.followDistance);
        bool Follow(Unit* target, float distance, float angle);
        float MoveDelay(float distance);
        bool IsMovingAllowed(Unit* target);
        bool IsMovingAllowed(uint32 mapId, float x, float y, float z);
        bool IsMovingAllowed();
        bool Flee(Unit *target, bool forced = false);
        void ClearIdleState();
        void UpdateMovementState();
        
        void CreateWp(Player* wpOwner, float x, float y, float z, float o, uint32 entry, bool important = false);
    };

    class FleeAction : public MovementAction
    {
    public:
        FleeAction(PlayerbotAI* ai, float distance = sPlayerbotAIConfig.spellDistance) : MovementAction(ai, "flee")
        {
			this->distance = distance;
		}

        virtual bool Execute(Event event);

	private:
		float distance;
    };

    class FleeWithPetAction : public MovementAction
    {
    public:
        FleeWithPetAction(PlayerbotAI* ai) : MovementAction(ai, "flee with pet") {}

        virtual bool Execute(Event event);
    };

    class RunAwayAction : public MovementAction
    {
    public:
        RunAwayAction(PlayerbotAI* ai) : MovementAction(ai, "runaway") {}
        virtual bool Execute(Event event);
    };

    class MoveToLootAction : public MovementAction
    {
    public:
        MoveToLootAction(PlayerbotAI* ai) : MovementAction(ai, "move to loot") {}
        virtual bool Execute(Event event);
        virtual bool isUseful();
    };

    class MoveOutOfEnemyContactAction : public MovementAction
    {
    public:
        MoveOutOfEnemyContactAction(PlayerbotAI* ai) : MovementAction(ai, "move out of enemy contact") {}
        virtual bool Execute(Event event);
        virtual bool isUseful();
    };

    class SetFacingTargetAction : public MovementAction
    {
    public:
        SetFacingTargetAction(PlayerbotAI* ai) : MovementAction(ai, "set facing") {}
        virtual bool Execute(Event event);
        virtual bool isUseful();
        virtual bool isPossible();
    };

    class SetBehindTargetAction : public MovementAction
    {
    public:
        SetBehindTargetAction(PlayerbotAI* ai) : MovementAction(ai, "set behind") {}
        virtual bool Execute(Event event);
        virtual bool isUseful();
        virtual bool isPossible();
    };

    class MoveOutOfCollisionAction : public MovementAction
    {
    public:
        MoveOutOfCollisionAction(PlayerbotAI* ai) : MovementAction(ai, "move out of collision") {}
        virtual bool Execute(Event event);
        virtual bool isUseful();
    };

    class MoveRandomAction : public MovementAction
    {
    public:
        MoveRandomAction(PlayerbotAI* ai) : MovementAction(ai, "move random") {}
        virtual bool Execute(Event event);
        virtual bool isUseful();
    };

}
