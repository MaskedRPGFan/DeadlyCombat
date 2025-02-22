#include <stddef.h>
#include "GetBlockCostHook.h"
#include "OnMeleeHitHook.h"
#include "PlayerUpdate.h"
#include "RegisterFuncs.hpp"

using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;

namespace {
    /**
     * Setup logging.
     */
    void InitializeLogging() {
        auto path = log_directory();
        if (!path) {
            report_and_fail("Unable to lookup SKSE logs directory.");
        }
        *path /= PluginDeclaration::GetSingleton()->GetName();
        *path += L".log";

        std::shared_ptr<spdlog::logger> log;
        if (IsDebuggerPresent()) {
            log = std::make_shared<spdlog::logger>("Global", std::make_shared<spdlog::sinks::msvc_sink_mt>());
        } else {
            log = std::make_shared<spdlog::logger>(
                "Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));
        }

#ifndef NDEBUG
        const auto level = spdlog::level::trace;
#else
        const auto level = spdlog::level::info;
#endif

        log->set_level(level);
        log->flush_on(level);

        spdlog::set_default_logger(std::move(log));
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v");
    }

    void MessageHandler(SKSE::MessagingInterface::Message* a_msg) {
        switch (a_msg->type) {
            case SKSE::MessagingInterface::kPostLoad: {
                // Only doing this here because it's important for the HitHook
                // to hook after Shield of Stamina
                log::trace("Initializing hooks...");
                BlockCostHook::GetBlockCostHook::InstallHook();
                HitHook::OnMeleeHit::InstallHook();
                PlayerUpdate::OnPlayerUpdate::InstallHook();
                log::trace("Hooks initialized.");
            } break;
        }
    }
}  // namespace

/**
 * This is the main callback for initializing the SKSE plugin, called just before Skyrim runs its main function.
 */
SKSEPluginLoad(const LoadInterface* skse) {
    InitializeLogging();

    auto* plugin = PluginDeclaration::GetSingleton();
    auto version = plugin->GetVersion();
    log::info("{} {} is loading...", plugin->GetName(), version);

    Init(skse);
    SKSE::GetMessagingInterface()->RegisterListener(MessageHandler);

    SKSE::GetPapyrusInterface()->Register(papyrus::RegisterFunctions);

    log::info("{} has finished loading.", plugin->GetName());
    return true;
}
