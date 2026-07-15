#include "core/Config.h"

#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <fstream>

namespace
{
    class TempConfigFile
    {
    public:
        TempConfigFile(const std::string& content)
        {
            _path = std::filesystem::temp_directory_path() / "didakt_test_config.json";
            std::ofstream file(_path);
            file << content;
        }

        ~TempConfigFile()
        {
            std::filesystem::remove(_path);
        }

        std::string PathString() const { return _path.string(); }

    private:
        std::filesystem::path _path;
    };

    void RequireDefaults(const Config& config)
    {
        Config defaults{};

        REQUIRE(config.window.title == defaults.window.title);
        REQUIRE(config.window.width == defaults.window.width);
        REQUIRE(config.window.height == defaults.window.height);

        REQUIRE(config.renderer.clearColor.R == defaults.renderer.clearColor.R);
        REQUIRE(config.renderer.clearColor.G == defaults.renderer.clearColor.G);
        REQUIRE(config.renderer.clearColor.B == defaults.renderer.clearColor.B);
        REQUIRE(config.renderer.clearColor.A == defaults.renderer.clearColor.A);
    }
}

TEST_CASE("Core.Config.LoadConfig_ParsesValidFile", "[Config]")
{
    TempConfigFile tempFile(R"({
        "window": {
            "title": "Test Window",
            "width": 1024,
            "height": 768
        },
        "renderer": {
            "clearColor": [10, 20, 30, 255]
        }
    })");

    Config config = LoadConfig(tempFile.PathString());

    REQUIRE(config.window.title == "Test Window");
    REQUIRE(config.window.width == 1024);
    REQUIRE(config.window.height == 768);

    REQUIRE(config.renderer.clearColor.R == 10);
    REQUIRE(config.renderer.clearColor.G == 20);
    REQUIRE(config.renderer.clearColor.B == 30);
    REQUIRE(config.renderer.clearColor.A == 255);
}

TEST_CASE("Core.Config.LoadConfig_MissingFileReturnsDefaults", "[Config]")
{
    Config config = LoadConfig("this/path/does/not/exist.json");
    RequireDefaults(config);
}

TEST_CASE("Core.Config.LoadConfig_MissingFieldFallsBackToDefault", "[Config]")
{
    Config defaults{};

    // "height" is absent — should silently take the struct default, not throw
    TempConfigFile tempFile(R"({
        "window": {
            "title": "Test Window",
            "width": 1024
        },
        "renderer": {
            "clearColor": [10, 20, 30, 255]
        }
    })");

    Config config = LoadConfig(tempFile.PathString());

    REQUIRE(config.window.title == "Test Window");
    REQUIRE(config.window.width == 1024);
    REQUIRE(config.window.height == defaults.window.height); // was missing
}

TEST_CASE("Core.Config.LoadConfig_MissingSectionFallsBackToDefaults", "[Config]")
{
    // "renderer" section entirely absent
    TempConfigFile tempFile(R"({
        "window": {
            "title": "Test Window",
            "width": 1024,
            "height": 768
        }
    })");

    Config config = LoadConfig(tempFile.PathString());

    REQUIRE(config.window.title == "Test Window");

    Config defaults{};
    REQUIRE(config.renderer.clearColor.R == defaults.renderer.clearColor.R);
    REQUIRE(config.renderer.clearColor.G == defaults.renderer.clearColor.G);
    REQUIRE(config.renderer.clearColor.B == defaults.renderer.clearColor.B);
    REQUIRE(config.renderer.clearColor.A == defaults.renderer.clearColor.A);
}

TEST_CASE("Core.Config.LoadConfig_MalformedJsonReturnsDefaults", "[Config]")
{
    TempConfigFile tempFile("{ this is not valid json ");

    Config config = LoadConfig(tempFile.PathString());
    RequireDefaults(config);
}

TEST_CASE("Core.Config.LoadConfig_TypeErrorReturnsDefaults", "[Config]")
{
    // "width" is a string instead of a number
    TempConfigFile tempFile(R"({
        "window": {
            "title": "Test Window",
            "width": "oops",
            "height": 768
        },
        "renderer": {
            "clearColor": [10, 20, 30, 255]
        }
    })");

    Config config = LoadConfig(tempFile.PathString());
    RequireDefaults(config); // whole-config fallback, confirmed manually
}

TEST_CASE("Core.Config.LoadConfig_MalformedClearColorArrayFallsBackToDefault", "[Config]")
{
    // clearColor array has only 3 elements instead of 4
    TempConfigFile tempFile(R"({
        "window": {
            "title": "Test Window",
            "width": 1024,
            "height": 768
        },
        "renderer": {
            "clearColor": [10, 20, 30]
        }
    })");

    Config config = LoadConfig(tempFile.PathString());

    REQUIRE(config.window.title == "Test Window"); // unaffected fields still parsed

    Config defaults{};
    REQUIRE(config.renderer.clearColor.R == defaults.renderer.clearColor.R);
    REQUIRE(config.renderer.clearColor.G == defaults.renderer.clearColor.G);
    REQUIRE(config.renderer.clearColor.B == defaults.renderer.clearColor.B);
    REQUIRE(config.renderer.clearColor.A == defaults.renderer.clearColor.A);
}