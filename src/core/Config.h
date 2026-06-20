#pragma once

#include <string>

struct RenderColor
{
    int R = 0;
    int G = 0;
    int B = 0;
    int A = 0;
};

struct WindowConfig
{
    std::string title = "Didakt";
    int width = 800;
    int height = 600;
};

struct RendererConfig
{
    RenderColor clearColor = { 0, 0, 0, 255 };
};

struct Config
{
    static constexpr const char* DEFAULT_PATH = "config/config.json";
 
    WindowConfig window;
    RendererConfig renderer;
};

Config LoadConfig(const std::string& path);