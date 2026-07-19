#pragma once

#include "interfaces/IProviderManager.h"

#include <memory>

std::unique_ptr<IProviderManager> CreateProviderManager();