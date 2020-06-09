
#include "ccli_pch.h"
#include "base.h"
#include "ccli.h"
#include "ccli_autocomplete.h"

#ifndef CCLI_COMPILED_LIB
#error Please define CCLI_COMPILE_LIB to compile this file.
#endif

// We add .inl into .cpp to create a entry point to build everything from.
#include "ccli_autocomplete.inl"
#include "ccli_system.inl"
#include "ccli_history.inl"
#include "ccli_command_data.inl"
