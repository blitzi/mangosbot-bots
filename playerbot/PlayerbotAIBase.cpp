#include "../botpch.h"
#include "playerbot.h"
#include "PlayerbotAIConfig.h"

using namespace ai;
using namespace std;

PlayerbotAIBase::PlayerbotAIBase()
{
}

void PlayerbotAIBase::UpdateAI(uint32 elapsed)
{
    UpdateAIInternal(elapsed);
}