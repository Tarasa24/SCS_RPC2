#pragma once

#include "types.h"
#include "activity_manager.h"

namespace discord {

class Core final {
public:
    static Result Create(ClientId clientId, std::uint64_t flags, Core** instance);

    ~Core();

    Result RunCallbacks();
    void SetLogHook(LogLevel minLevel, std::function<void(LogLevel, char const*)> hook);

    discord::ActivityManager& ActivityManager();

private:
    Core() = default;
    Core(Core const& rhs) = delete;
    Core& operator=(Core const& rhs) = delete;
    Core(Core&& rhs) = delete;
    Core& operator=(Core&& rhs) = delete;

    IDiscordCore* internal_;
    Event<LogLevel, char const*> setLogHook_;
    discord::ActivityManager activityManager_;
};

} // namespace discord
