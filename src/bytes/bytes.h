/*
This file is part of Nameplate.
Copyright (C) 2024 BunnyBox Productions

Nameplate is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Nameplate is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Nameplate.
If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef BYTES__BYTES_H
#define BYTES__BYTES_H

#include <inttypes.h>

[[maybe_unused]] static inline float i2f(uint32_t i) {
    union p {
        uint32_t i;
        float f;
    };
    return ((union p){ .i = i }).f;
}

#define GET_VARARGS_MACRO3(_1, _2, _3, MACRO_NAME, ...) MACRO_NAME

#define ptr8_(x)           ((uint8_t*)(&((x))[0]))
#define cptr8_(x)          ((char*) ptr8_(x))
#define lsb8_(x)           ((uint8_t)(((x))[0]))
#define lsb16_(x)          ((uint16_t)(((x))[0] | (((x))[1] << 8)))
#define lsb24_(x)          ((uint32_t)(((x))[0] | (((x))[1] << 8) | (((x))[2] << 16)))
#define lsb32_(x)          ((uint32_t)(((x))[0] | (((x))[1] << 8) | (((x))[2] << 16) | (((x))[3] << 24)))
#define lsbf32_(x)         i2f(lsb32_(x))

#define msb32_(x)          ((uint32_t)(((x))[3] | (((x))[2] << 8) | (((x))[1] << 16) | (((x))[0] << 24)))

#define ptr8_i(x, o)       ((uint8_t*)(&((x)+(o))[0]))
#define cptr8_i(x, o)      ((char*) ptr8_i(x, o))
#define lsb8_i(x, o)       ((uint8_t)(((x)+(o))[0]))
#define lsb16_i(x, o)      ((uint16_t)(((x)+(o))[0] | (((x)+(o))[1] << 8)))
#define lsb24_i(x, o)      ((uint32_t)(((x)+(o))[0] | (((x)+(o))[1] << 8) | (((x)+(o))[2] << 16)))
#define lsb32_i(x, o)      ((uint32_t)(((x)+(o))[0] | (((x)+(o))[1] << 8) | (((x)+(o))[2] << 16) | (((x)+(o))[3] << 24)))
#define lsbf32_i(x, o)     i2f(lsb32_i(x, o))

#define msb32_i(x, o)      ((uint32_t)(((x)+(o))[3] | (((x)+(o))[2] << 8) | (((x)+(o))[1] << 16) | (((x)+(o))[0] << 24)))

#define ptr8_bi(x, b, o)   ((uint8_t*)(&((x)+(b)+(o))[0]))
#define cptr8_bi(x, b, o)  ((char*) ptr8_bi(x, b, o))
#define lsb8_bi(x, b, o)   ((uint8_t)(((x)+(b)+(o))[0]))
#define lsb16_bi(x, b, o)  ((uint16_t)(((x)+(b)+(o))[0] | (((x)+(b)+(o))[1] << 8)))
#define lsb24_bi(x, b, o)  ((uint32_t)(((x)+(b)+(o))[0] | (((x)+(b)+(o))[1] << 8) | (((x)+(b)+(o))[2] << 16)))
#define lsb32_bi(x, b, o)  ((uint32_t)(((x)+(b)+(o))[0] | (((x)+(b)+(o))[1] << 8) | (((x)+(b)+(o))[2] << 16) | (((x)+(b)+(o))[3] << 24)))
#define lsbf32_bi(x, b, o) i2f(lsb32_bi(x, b, o))

#define msb32_bi(x, b, o)  ((uint32_t)(((x)+(b)+(o))[3] | (((x)+(b)+(o))[2] << 8) | (((x)+(b)+(o))[1] << 16) | (((x)+(b)+(o))[0] << 24)))

#define ptr8(...)          GET_VARARGS_MACRO3(__VA_ARGS__, ptr8_bi, ptr8_i, ptr8_, nothing)(__VA_ARGS__)
#define cptr8(...)         GET_VARARGS_MACRO3(__VA_ARGS__, cptr8_bi, cptr8_i, cptr8_, nothing)(__VA_ARGS__)
#define lsb8(...)          GET_VARARGS_MACRO3(__VA_ARGS__, lsb8_bi, lsb8_i, lsb8_, nothing)(__VA_ARGS__)
#define lsb16(...)         GET_VARARGS_MACRO3(__VA_ARGS__, lsb16_bi, lsb16_i, lsb16_, nothing)(__VA_ARGS__)
#define lsb24(...)         GET_VARARGS_MACRO3(__VA_ARGS__, lsb24_bi, lsb24_i, lsb24_, nothing)(__VA_ARGS__)
#define lsb32(...)         GET_VARARGS_MACRO3(__VA_ARGS__, lsb32_bi, lsb32_i, lsb32_, nothing)(__VA_ARGS__)
#define lsbf32(...)        GET_VARARGS_MACRO3(__VA_ARGS__, lsbf32_bi, lsbf32_i, lsbf32_, nothing)(__VA_ARGS__)

#define msb32(...)         GET_VARARGS_MACRO3(__VA_ARGS__, msb32_bi, msb32_i, msb32_, nothing)(__VA_ARGS__)

// Signed variations
#define slsb8(...)         ((int8_t) (GET_VARARGS_MACRO3(__VA_ARGS__, lsb8_bi, lsb8_i, lsb8_, nothing)(__VA_ARGS__)))
#define slsb16(...)        ((int16_t) (GET_VARARGS_MACRO3(__VA_ARGS__, lsb16_bi, lsb16_i, lsb16_, nothing)(__VA_ARGS__)))
#define slsb24(...)        ((int32_t) (GET_VARARGS_MACRO3(__VA_ARGS__, lsb24_bi, lsb24_i, lsb24_, nothing)(__VA_ARGS__)))
#define slsb32(...)        ((int32_t) (GET_VARARGS_MACRO3(__VA_ARGS__, lsb32_bi, lsb32_i, lsb32_, nothing)(__VA_ARGS__)))

#define smsb32(...)        ((int32_t) (GET_VARARGS_MACRO3(__VA_ARGS__, msb32_bi, msb32_i, msb32_, nothing)(__VA_ARGS__)))

#define plsb32(...)        ((uintptr_t) (GET_VARARGS_MACRO3(__VA_ARGS__, lsb32_bi, lsb32_i, lsb32_, nothing)(__VA_ARGS__)))

#endif
