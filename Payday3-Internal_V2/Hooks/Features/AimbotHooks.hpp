#pragma once

#include "pch.h"

class AimbotHooks
{
public:
	bool Setup();
	void Destroy();
};

inline std::unique_ptr<AimbotHooks> aimbotHooks = std::make_unique<AimbotHooks>();