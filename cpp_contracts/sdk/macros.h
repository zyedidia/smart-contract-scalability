#pragma once

#define MODULE __attribute((import_module("scs")))
#define NAME(x) __attribute((import_name(x)))

#define BUILTIN(x) MODULE NAME(x)

#define EXPORT(name) void __attribute((export_name(name)))

