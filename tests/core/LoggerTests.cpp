#include "core/Logger.h"

#include <catch2/catch_test_macros.hpp>

namespace
{
    class MockLoggingProvider : public ILoggingProvider
    {
    public:
        void Log(LogLevel level, const std::string& message) override
        {
            lastLevel = level;
            lastMessage = message;
            callCount++;
        }

        LogLevel lastLevel = LogLevel::Debug;
        std::string lastMessage;
        int callCount = 0;
    };
}

TEST_CASE("Core.Logger.Log_ForwardsToProvider", "[Logger]")
{
    MockLoggingProvider provider;
    Logger::SetProvider(provider);

    Logger::Log(LogLevel::Warning, "test message");

    REQUIRE(provider.callCount == 1);
    REQUIRE(provider.lastLevel == LogLevel::Warning);
    REQUIRE(provider.lastMessage == "test message");
}

TEST_CASE("Core.Logger.Log_FormatsArguments", "[Logger]")
{
    MockLoggingProvider provider;
    Logger::SetProvider(provider);

    Logger::Log(LogLevel::Error, "value is %d, name is %s", 42, "test");

    REQUIRE(provider.callCount == 1);
    REQUIRE(provider.lastLevel == LogLevel::Error);
    REQUIRE(provider.lastMessage == "value is 42, name is test");
}

TEST_CASE("Core.Logger.Log_PassesCorrectLevel", "[Logger]")
{
    MockLoggingProvider provider;
    Logger::SetProvider(provider);

    Logger::Log(LogLevel::Critical, "critical message");

    REQUIRE(provider.lastLevel == LogLevel::Critical);
}

TEST_CASE("Core.Logger.Log_MultipleCallsEachForward", "[Logger]")
{
    MockLoggingProvider provider;
    Logger::SetProvider(provider);

    Logger::Log(LogLevel::Info, "first");
    Logger::Log(LogLevel::Info, "second");
    Logger::Log(LogLevel::Info, "third");

    REQUIRE(provider.callCount == 3);
    REQUIRE(provider.lastMessage == "third"); // mock only tracks the most recent
}

TEST_CASE("Core.Logger.SetProvider_SwitchesActiveProvider", "[Logger]")
{
    MockLoggingProvider providerA;
    MockLoggingProvider providerB;

    Logger::SetProvider(providerA);
    Logger::Log(LogLevel::Info, "goes to A");

    Logger::SetProvider(providerB);
    Logger::Log(LogLevel::Info, "goes to B");

    REQUIRE(providerA.callCount == 1);
    REQUIRE(providerB.callCount == 1);
    REQUIRE(providerA.lastMessage == "goes to A");
    REQUIRE(providerB.lastMessage == "goes to B");
}