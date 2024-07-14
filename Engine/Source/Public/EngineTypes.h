#pragma once
#include "Debugging/SDebug.h"

// System Libs
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>

// redefine of the standard string
typedef std::string SString;

// redefine all of the unsigned ints
typedef uint8_t SUi8;
typedef uint16_t SUi16;
typedef uint32_t SUi32;
typedef uint64_t SUi64;

// redefine the standard vector
template <typename T>
using TArray = std::vector<T>;

// redefine smart pointer
// redefine shared pointer
template <typename T>
using TShared = std::shared_ptr<T>;

// redefine unique pointer
template <typename T>
using TUnique= std::unique_ptr<T>;

// redefine weak pointer
template <typename T>
using TWeak = std::weak_ptr<T>;

// redefine the make shared function
template <typename T, typename... Args>
TShared<T> TMakeShared(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}

// redefine the make unique function
template <typename T, typename... Args>
TUnique<T> TMakeUnique(Args&&... args) {
	return std::make_unique<T>(std::forward<Args>(args)...);
}