#pragma once

#include "ZGEDecl.h"

#include "Pattern/Singleton.h"

namespace ZGE
{
    // Hardware Abstract Layer
    class RenderDeviceHAL
        : public Singleton<RenderDeviceHAL>
    {
    public:
        enum class DeviceMinRequirement
        {
            OPENGL_MAJOR_VERSION = 4,
            OPENGL_MINOR_VERSION = 0,
        };

        enum class DeviceCap
        {
            OPENGL_MAJOR_VERSION,
            OPENGL_MINOR_VERSION,
            MAX_VERTEX_ATTRIBS,
            MAX_VERTEX_UNIFORM_COMPONENTS,
            MAX_VERTEX_UNIFORM_VECTORS,
            MAX_VERTEX_OUTPUT_COMPONENTS,
            MAX_FRAGMENT_INPUT_COMPONENTS,
            MAX_FRAGMENT_UNIFORM_COMPONENTS,
            MAX_FRAGMENT_UNIFORM_VECTORS,
        };
    };
}