#pragma once

#include <string>

class IWindowProvider
{
public:
    virtual ~IWindowProvider() = default;

    virtual void SetTitle(const std::string& title) = 0;
};