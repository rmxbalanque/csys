#ifndef CCLI_SCRIPT_H
#define CCLI_SCRIPT_H
#pragma once

#include <string>
#include <vector>
#include "ccli/ccli_api.h"

namespace ccli
{
	class CCLI_API Script
	{
	public:

		/*!
		 * \brief Create script object from file
		 * \param path Path of script file
		 * \param load_on_init Load script when object is created
		 */
		explicit Script(std::string  path, bool load_on_init = true);

		/*!
		 * \brief Create script object from file
		 * \param path Path of script file
		 * \param load_on_init Load script when object is created
		 */
		explicit Script(const char * path, bool load_on_init = true);

		/*!
		 * \brief Create script object from file already in memory
		 * \param data Script file memory
		 */
		explicit Script(std::vector<std::string> data);

		/*!
		 * \brief Load script file
		 */
		void load();

		/*!
		 * \brief Reload script file (Unload & Load)
		 */
		void reload();

		/*!
		 * /brief Clear script data
		 */
		void unload();

		/*!
		 * \brief Set script file path (Will be used when laoding)
		 * \param path Script file path
		 */
		void set_path(std::string path);

		/*!
		 * \brief Retrieve script data (Commands)
		 * \return List of commands in script
		 */
		const std::vector<std::string> & data();

	private:
		std::vector<std::string> m_Data;	//!< Commands in script
		std::string m_Path;					//!< Path of script file
		bool m_FromMemory;					//!< Flag to specify if script was loaded from file or memory
	};
}

#ifdef CCLI_HEADER_ONLY
#include "ccli/ccli_script.inl"
#endif

#endif
