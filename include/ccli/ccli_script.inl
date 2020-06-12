
#pragma once

#ifndef CCLI_HEADER_ONLY
#include "ccli_script.h"
#endif

#include <fstream>
//#include "ccli_system.h"

namespace ccli
{
	CCLI_INLINE Script::Script(std::string_view path, bool load_on_init) : m_Path(path)
	{
		// Load file.
		if (load_on_init && std::filesystem::exists(path) && !std::filesystem::is_empty(path))
		{
			load();
		}
	}

	CCLI_INLINE void Script::load()
	{
		std::ifstream script_fstream(m_Path);

		// Check and open file.
		if (script_fstream.good() && script_fstream.is_open())
		{
			// Line buffer.
			std::string line_buf;

			// Read commands.
			while (std::getline(script_fstream, line_buf))
			{
				m_Data.push_back(line_buf);
			}

			// Close file.
			script_fstream.close();
		}
	}

	CCLI_INLINE void Script::reload()
	{
		m_Data.clear();

		load();
	}

	CCLI_INLINE const std::vector<std::string> &Script::data()
	{
		return m_Data;
	}
}
