#pragma once

struct TextureHandle
{
    void* handle = nullptr;
    
    bool IsValid() const { return handle != nullptr; }
};