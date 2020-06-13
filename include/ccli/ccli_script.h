#ifndef TEQUILA_CCLI_SCRIPT_H
#define TEQUILA_CCLI_SCRIPT_H
#pragma once

#include "ccli_pch.h"
#include "ccli_api.h"
#include <filesystem>

namespace ccli
{
	class CCLI_API Script
	{
	public:
		explicit Script(const std::string & path, bool load_on_init = true);
		explicit Script(const char * path, bool load_on_init = true);

		void load();

		void reload();

		const std::vector<std::string> & data();

	private:
		std::vector<std::string> m_Data;
		std::filesystem::path m_Path;
	};
}

#ifdef CCLI_HEADER_ONLY
#include "ccli_script.inl"
#endif

#endif //TEQUILA_CCLI_SCRIPT_H
