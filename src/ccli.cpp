
#ifndef CCLI_COMPILED_LIB
#error Please define CCLI_COMPILE_LIB to compile this file.
#endif

// We add .inl into .cpp to create a entry point to build everything from.
#include "ccli_autocomplete.inl"
#include "ccli_system.inl"
#include "ccli_history.inl"
#include "ccli_item.inl"
#include "ccli_script.inl"
