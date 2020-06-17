
#pragma once

#ifndef CCLI_HEADER_ONLY
#include "ccli_script.h"
#endif

#include <fstream>
#include "ccli_exceptions.h"

namespace ccli
{

	CCLI_INLINE Script::Script(std::string path, bool load_on_init) : m_Path(std::move(path))
	{
		// Load file.
		if (load_on_init)
			load();
	}

	CCLI_INLINE Script::Script(const char *path, bool load_on_init) : m_Path(path)
	{
		// Load file.
		if (load_on_init)
			load();
	}

	CCLI_INLINE void Script::load()
	{
		std::ifstream script_fstream(m_Path);

		// Error check.
		if (!script_fstream.good())
			throw ccli::Exception("Failed to load script", m_Path);

		// Check and open file.
		if (script_fstream.good() && script_fstream.is_open())
		{
			// Line buffer.
			std::string line_buf;

			// Read commands.
			while (std::getline(script_fstream, line_buf))
			{
				m_Data.emplace_back(line_buf);
			}

			// Close file.
			script_fstream.close();
		}
	}

	CCLI_INLINE void Script::reload()
	{
		unload();
		load();
	}

	CCLI_INLINE void Script::unload()
	{
		m_Data.clear();
	}

	CCLI_INLINE const std::vector<std::string> &Script::data()
	{
		return m_Data;
	}
}
