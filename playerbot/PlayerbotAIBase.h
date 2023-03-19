#pragma once

class Player;
class PlayerbotMgr;
class ChatHandler;
class PerformanceMonitorOperation;

using namespace std;

class PlayerbotAIBase
{
public:
    PlayerbotAIBase();

public:
    virtual void UpdateAI(uint32 elapsed);

protected:
    virtual void UpdateAIInternal(uint32 elapsed, bool minimal = false) = 0;

protected:
    PerformanceMonitorOperation* totalPmo = nullptr;
};
