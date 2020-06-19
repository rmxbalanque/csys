#ifndef CCLI_SCRIPT_H
#define CCLI_SCRIPT_H
#pragma once

#include "ccli/ccli_pch.h"
#include "ccli/ccli_api.h"

namespace ccli
{
	class CCLI_API Script
	{
	public:

		/*!
		 * \brief Create script object
		 * \param path Path of script file
		 * \param load_on_init Load script when object is created
		 */
		explicit Script(std::string  path, bool load_on_init = true);

		/*!
		 * \brief Create script object
		 * \param path Path of script file
		 * \param load_on_init Load script when object is created
		 */
		explicit Script(const char * path, bool load_on_init = true);

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
		 * \brief Retrieve script data (Commands)
		 * \return List of commands in script
		 */
		const std::vector<std::string> & data();

	private:
		std::vector<std::string> m_Data;	//!< Commands in script
		std::string m_Path;					//!< Path of script file
	};
}

#ifdef CCLI_HEADER_ONLY
#include "ccli/ccli_script.inl"
#endif

#endif
