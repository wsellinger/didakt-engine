#include "Config.h"
#include <nlohmann/json.hpp>
#include <fstream>

#include <SDL_log.h>

using string = std::string;
using ifstream = std::ifstream;
using json = nlohmann::json;

Config LoadConfig(const string& path)
{
    Config config;

    //Get Config File
    ifstream file(path);
    if (!file.is_open())
        return config;

    try
    {
        json configJson = json::parse(file);

        //Window
        json windowJson = configJson.value("window", json::object());
        config.window.title = windowJson.value("title", config.window.title);
        config.window.width = windowJson.value("width", config.window.width);
        config.window.height = windowJson.value("height", config.window.height);

        //Renderer
        json rendererJson = configJson.value("renderer", json::object());
        json clearColorJson = rendererJson.value("clearColor", json::array());
        if (clearColorJson.is_array() && clearColorJson.size() == 4)
        {
            config.renderer.clearColor.R = clearColorJson[0].get<int>();
            config.renderer.clearColor.G = clearColorJson[1].get<int>();
            config.renderer.clearColor.B = clearColorJson[2].get<int>();
            config.renderer.clearColor.A = clearColorJson[3].get<int>();
        }
    }
    catch (const json::exception& e)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Config: failed to parse '%s': %s", path.c_str(), e.what());
        return Config{};
    }

    return config;
}