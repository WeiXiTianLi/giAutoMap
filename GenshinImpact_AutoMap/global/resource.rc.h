#pragma once
#include "resource.include.h"
#include <Windows.h>

class resource_rc :public resource_image
{
public:
    resource_rc();
    ~resource_rc() override = default;
public:
    void load() override;
};