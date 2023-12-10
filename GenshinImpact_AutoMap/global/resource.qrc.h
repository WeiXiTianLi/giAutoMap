#pragma once
#include "resource.include.h"
#include <Windows.h>

class resource_qrc :public resource_image
{
public:
    resource_qrc();
    ~resource_qrc() override = default ;
public:
    void load() override;

};