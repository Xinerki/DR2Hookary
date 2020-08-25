// Precompiled header generator
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

// Macros and types
#include "Version.hpp"
#include "Types.hpp"
#include "../Hooking/Macros.hpp"

// Actual headers
#include "../Hooking/Container/HashMap.hpp"
#include "../Hooking/Container/HashSet.hpp"
#include "../Hooking/Container/Sort.hpp"
#include "../Hooking/Container/Str.hpp"
