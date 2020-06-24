// Copyright (c) 2020-present, Roland Munguia & Tristan Florian Bouchard.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#ifndef CSYS_HEADER_ONLY

#include "csys/script.h"

#endif

#include <fstream>
#include <utility>
#include "csys/exceptions.h"

namespace csys
{

	CSYS_INLINE Script::Script(std::string path, bool load_on_init) : m_Path(std::move(path)), m_FromMemory(false)
	{
		// Load file.
		if (load_on_init)
			load();
	}

	CSYS_INLINE Script::Script(const char *path, bool load_on_init) : m_Path(path), m_FromMemory(false)
	{
		// Load file.
		if (load_on_init)
			load();
	}

	CSYS_INLINE Script::Script(std::vector<std::string> data) : m_Data(std::move(data)), m_FromMemory(true)
	{}

	CSYS_INLINE void Script::load()
	{
		std::ifstream script_fstream(m_Path);

		// Error check.
		if (!script_fstream.good())
			throw csys::Exception("Failed to load script", m_Path);

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

	CSYS_INLINE void Script::reload()
	{
		if (m_FromMemory) return;

		unload();
		load();
	}

	CSYS_INLINE void Script::unload()
	{
		m_Data.clear();
	}

	CSYS_INLINE void Script::set_path(std::string path)
	{
		m_Path = std::move(path);
	}

	CSYS_INLINE const std::vector<std::string> &Script::data()
	{
		return m_Data;
	}
}
