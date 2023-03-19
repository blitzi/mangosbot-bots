#pragma once
#include "GenericActions.h"

namespace ai
{
	class ChangeCombatStrategyAction : public ChatCommandAction
    {
	public:
		ChangeCombatStrategyAction(PlayerbotAI* ai, string name = "co") : ChatCommandAction(ai, name) {}
        virtual bool Execute(Event& event) override;
        virtual bool IgnoresCasting() { return true; }
    };

    class ChangeNonCombatStrategyAction : public ChatCommandAction
    {
    public:
        ChangeNonCombatStrategyAction(PlayerbotAI* ai) : ChatCommandAction(ai, "nc") {}
        virtual bool Execute(Event& event) override;
        virtual bool IgnoresCasting() { return true; }
    };

    class ChangeDeadStrategyAction : public ChatCommandAction
    {
    public:
        ChangeDeadStrategyAction(PlayerbotAI* ai) : ChatCommandAction(ai, "de") {}
        virtual bool Execute(Event& event) override;
        virtual bool IgnoresCasting() { return true; }
    };

    class ChangeAllStrategyAction : public ChatCommandAction
    {
    public:
        ChangeAllStrategyAction(PlayerbotAI* ai, string name = "change strategy from all", string strategy = "") : ChatCommandAction(ai, name), strategy(strategy) {}
        virtual bool Execute(Event& event) override;
        virtual bool IgnoresCasting() { return true; }

    private:
        string strategy;
    };
}
