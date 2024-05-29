// Copyright Ryu KeunBeom, Inc. All Rights Reserved.

#pragma once

#include <type_traits>

// 덧셈 및 뺄셈 연산자 정의를 위한 매크로
#define ENUM_CLASS_OPERATORS(Enum) \
    inline Enum& operator+=(Enum& Lhs, int32 Rhs) { return Lhs = static_cast<Enum>(static_cast<std::underlying_type_t<Enum>>(Lhs) + Rhs); } \
    inline Enum& operator-=(Enum& Lhs, int32 Rhs) { return Lhs = static_cast<Enum>(static_cast<std::underlying_type_t<Enum>>(Lhs) - Rhs); } \
    inline constexpr Enum operator+(Enum Lhs, int32 Rhs) { return static_cast<Enum>(static_cast<std::underlying_type_t<Enum>>(Lhs) + Rhs); } \
    inline constexpr Enum operator-(Enum Lhs, int32 Rhs) { return static_cast<Enum>(static_cast<std::underlying_type_t<Enum>>(Lhs) - Rhs); } \
    inline constexpr Enum operator+(int32 Lhs, Enum Rhs) { return static_cast<Enum>(Lhs + static_cast<std::underlying_type_t<Enum>>(Rhs)); } \
    inline constexpr Enum operator-(int32 Lhs, Enum Rhs) { return static_cast<Enum>(Lhs - static_cast<std::underlying_type_t<Enum>>(Rhs)); } \
    inline constexpr int32 operator+(Enum Lhs, Enum Rhs) { return static_cast<int32>(static_cast<std::underlying_type_t<Enum>>(Lhs) + static_cast<std::underlying_type_t<Enum>>(Rhs)); } \
    inline constexpr int32 operator-(Enum Lhs, Enum Rhs) { return static_cast<int32>(static_cast<std::underlying_type_t<Enum>>(Lhs) - static_cast<std::underlying_type_t<Enum>>(Rhs)); } \
    inline constexpr Enum operator++(Enum& E) { return E = static_cast<Enum>(static_cast<std::underlying_type_t<Enum>>(E) + 1); } \
    inline constexpr Enum operator--(Enum& E) { return E = static_cast<Enum>(static_cast<std::underlying_type_t<Enum>>(E) - 1); }