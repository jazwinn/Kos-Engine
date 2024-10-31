#pragma once
#include <cstdint>

using keyCode = uint16_t;

namespace keys {
    enum : keyCode {
        // From glfw3.h
        LMB = 0,
        SPACE = 32,

        NUM0 = 48, /* 0 */

        A = 65,
        D = 68,
        E = 69,
        R = 82,
        S = 83,
        W = 87,


        ESC = 256,
        F11 = 300,

    };
}