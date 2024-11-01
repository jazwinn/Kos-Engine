#pragma once
#include <cstdint>

using keyCode = uint16_t;

namespace keys {
    enum : keyCode {
        // From glfw3.h
        LMB = 0,
        SPACE = 32,

        NUM0 = 48, /* 0 */
        NUM1 = 49, /* 1 */
        NUM2 = 50, /* 2 */
        NUM3 = 51, /* 3 */
        NUM4 = 52, /* 4 */
        NUM5 = 53, /* 5 */
        NUM6 = 54, /* 6 */
        NUM7 = 55, /* 7 */
        NUM8 = 56, /* 8 */
        NUM9 = 57, /* 9 */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,


        ESC = 256,
		ENTER = 257,
		TAB = 258,
		BACKSPACE = 259,
		RIGHT = 262,
		LEFT = 263,
		DOWN = 264,
		UP = 265,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,

    };
}