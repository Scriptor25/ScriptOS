#pragma once

#define NORETURN __attribute__((noreturn))

#define offsetof(type, name) __builtin_offsetof(type, name)
