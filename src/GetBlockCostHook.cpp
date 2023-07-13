#include "GetBlockCostHook.h"

#include <SKSE/SKSE.h>

#include "PlayerUpdate.h"

using namespace BlockCostHook;

GetBlockCostHook& GetBlockCostHook::GetSingleton() noexcept {
    static GetBlockCostHook instance;
    return instance;
}

void GetBlockCostHook::InstallHook() {
    SKSE::AllocTrampoline(1 << 4);

    REL::Relocation<uintptr_t> hook{RELOCATION_ID(37633, 38586)};
    auto& trampoline = SKSE::GetTrampoline();
    _GetBlockCost = trampoline.write_call<5>(hook.address() + REL::Relocate(0x8D4, 0xB39), GetBlockCost);
}

float GetBlockCostHook::GetBlockCost(RE::HitData& hitData) {
    // Sanity check: should probably always be true if this function is getting called
    if (hitData.flags.any(RE::HitData::Flag::kBlocked)) {
        auto victim = hitData.target.get().get();
        const auto& singleton = PlayerUpdate::OnPlayerUpdate::GetSingleton();
        if (victim && victim->IsPlayerRef() && singleton.enableTimedBlocking) {
            const float timeSpentBlocking = singleton.playerTimeSpentBlocking;
            if (timeSpentBlocking > 0.f && timeSpentBlocking < singleton.timedBlockingPeriod) {
                auto aggressor = hitData.aggressor.get().get();
                if (aggressor && aggressor->HasKeywordString("ActorTypeNPC")) {
                    // Timed block of player against NPC: no Stamina cost
                    return 0.f;
                }
            }
        }
    }

    // Call the normal game's code
    return _GetBlockCost(hitData);
}