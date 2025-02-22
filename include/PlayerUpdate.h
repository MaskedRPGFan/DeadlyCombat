#pragma once

#include <RE/Skyrim.h>

namespace PlayerUpdate {
#pragma warning(push)
#pragma warning(disable : 4251)

    class __declspec(dllexport) OnPlayerUpdate {
    public:
        [[nodiscard]] static OnPlayerUpdate& GetSingleton() noexcept;

        static void InstallHook();

        /** Time player spent blocking. */
        float playerTimeSpentBlocking = 0.f;

        /** Time player cooling down after blocking. */
        float playerTimeSpentCoolingDown = 0.f;

        /** Timed blocking period. */
        float timedBlockingPeriod = 0.25;

        /** Cooldown period. */
        float cooldownPeriod = 0.25;

        bool enableTimedBlocking = true;

    private:
        OnPlayerUpdate() = default;
        OnPlayerUpdate(const OnPlayerUpdate&) = delete;
        OnPlayerUpdate(OnPlayerUpdate&&) = delete;
        ~OnPlayerUpdate() = default;

        OnPlayerUpdate& operator=(const OnPlayerUpdate&) = delete;
        OnPlayerUpdate& operator=(OnPlayerUpdate&&) = delete;

        /** Our new function */
        static void Update(RE::PlayerCharacter* player, float delta);

        /** The original function */
        static inline REL::Relocation<decltype(Update)> _Update;
    };

#pragma warning(pop)
}  // namespace PlayerUpdate