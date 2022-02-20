#pragma once

#include "../Trigger.h"

namespace ai
{
    class WorldPacketTrigger : public Trigger {
    public:
        WorldPacketTrigger(PlayerbotAI* ai, string command) : Trigger(ai, command), triggered(false) {}

        virtual void ExternalEvent(WorldPacket &packet, Player* owner = NULL)
        {
            this->packet = packet;
            this->owner = owner;
            triggered = true;
        }

        virtual Event Check()
        {
            if (!triggered)
                return Event();

            return Event(GetName(), packet, owner);
        }

        virtual bool CheckOnMinimal() { return true; }


        virtual void Reset()
        {
            triggered = false;
        }

    private:
        WorldPacket packet;
        bool triggered;
        Player* owner;
    };
}
