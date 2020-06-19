
#ifndef CCLI_COMPILED_LIB
#error Please define CCLI_COMPILE_LIB to compile this file.
#endif

// We add .inl into .cpp to create a entry point to build everything from.
#include "ccli/ccli_autocomplete.inl"
#include "ccli/ccli_system.inl"
#include "ccli/ccli_history.inl"
#include "ccli/ccli_item.inl"
#include "ccli/ccli_script.inl"
