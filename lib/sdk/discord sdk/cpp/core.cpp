#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "core.h"

#include <cstring>
#include <memory>

namespace discord {

Result Core::Create(ClientId clientId, std::uint64_t flags, Core** instance)
{
    if (!instance) {
        return Result::InternalError;
    }

    (*instance) = new Core();
    DiscordCreateParams params{};
    DiscordCreateParamsSetDefault(&params);
    params.client_id = clientId;
    params.flags = flags;
    params.events = nullptr;
    params.event_data = *instance;
    params.activity_events = &ActivityManager::events_;
    auto result = DiscordCreate(DISCORD_VERSION, &params, &((*instance)->internal_));
    if (result != DiscordResult_Ok || !(*instance)->internal_) {
        delete (*instance);
        (*instance) = nullptr;
    }

    return static_cast<Result>(result);
}

Core::~Core()
{
    if (internal_) {
        internal_->destroy(internal_);
        internal_ = nullptr;
    }
}

Result Core::RunCallbacks()
{
    auto result = internal_->run_callbacks(internal_);
    return static_cast<Result>(result);
}

void Core::SetLogHook(LogLevel minLevel, std::function<void(LogLevel, char const*)> hook)
{
    setLogHook_.DisconnectAll();
    setLogHook_.Connect(std::move(hook));
    static auto wrapper =
      [](void* callbackData, EDiscordLogLevel level, char const* message) -> void {
        auto cb(reinterpret_cast<decltype(setLogHook_)*>(callbackData));
        if (!cb) {
            return;
        }
        (*cb)(static_cast<LogLevel>(level), static_cast<const char*>(message));
    };

    internal_->set_log_hook(
      internal_, static_cast<EDiscordLogLevel>(minLevel), &setLogHook_, wrapper);
}

discord::ActivityManager& Core::ActivityManager()
{
    if (!activityManager_.internal_) {
        activityManager_.internal_ = internal_->get_activity_manager(internal_);
    }

    return activityManager_;
}

} // namespace discord
