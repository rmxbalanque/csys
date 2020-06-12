#ifndef TEQUILA_CCLI_SCRIPT_H
#define TEQUILA_CCLI_SCRIPT_H
#pragma once

#include "ccli_pch.h"
#include "ccli_api.h"
#include <filesystem>

namespace ccli
{
	//class System;

	class CCLI_API Script
	{
	public:
		explicit Script(std::string_view path, bool load_on_init = true);

		void load();

		void reload();

		const std::vector<std::string> & data();

		//	void run(System & sys);

	private:
		std::vector<std::string> m_Data;
		std::filesystem::path m_Path;
	};
}

#ifdef CCLI_HEADER_ONLY
#include "ccli_script.inl"
#endif

#endif //TEQUILA_CCLI_SCRIPT_H
