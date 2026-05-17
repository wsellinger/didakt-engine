#include "Config.h"
#include <nlohmann/json.hpp>
#include <fstream>

using string = std::string;
using ifstream = std::ifstream;
using json = nlohmann::json;

Config LoadConfig(const string& path)
{
    Config config;

    //Get Config File //TODO changes to config file still do not get picked up by CMake and sent to build folder
    ifstream file(path);
    if (!file.is_open())
        return config;

    json configJson = json::parse(file); 

    //Build Config

    //Window
    config.window.title = configJson["window"]["title"].get<string>();
    config.window.width = configJson["window"]["width"].get<int>();
    config.window.height = configJson["window"]["height"].get<int>();

    //Renderer
    json clearColor = configJson["renderer"]["clearColor"];
    config.renderer.clearColor.R = clearColor[0].get<int>();
    config.renderer.clearColor.G = clearColor[1].get<int>();
    config.renderer.clearColor.B = clearColor[2].get<int>();
    config.renderer.clearColor.A = clearColor[3].get<int>();

    return config;
}
