#pragma once

#ifndef CCLI_HEADER_ONLY
#include "ccli_system.h"
#endif

namespace ccli
{
	///////////////////////////////////////////////////////////////////////////
	// Public methods /////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	// Commands-Error-Warning strings.
	static constexpr std::string_view s_Set = "set";
	static constexpr std::string_view s_Get = "get";
	static constexpr std::string_view s_ErrorNoVar = "No variable provided";
	static constexpr std::string_view s_ErrorBadArg = "No a valid argument";
	static constexpr std::string_view s_ErrorNoArg = "No argument provided";
	static constexpr std::string_view s_WarnMoreArgs = "More arguments than required were specified";
	static constexpr std::string_view s_ErrorSetGetNotFound = "Command doesn't exists and/or variable is not registered";
	static constexpr std::string_view s_ErrorCmdNotFound = "Command doesn't exist";

	CCLI_INLINE void System::parse(const std::string &line)
	{
		// Error.
		if (line.empty())
			return;

		// Push to history.
		m_CommandHistory.push_back(line);

		// Find command string.
		size_t cmd_pos = line.find_first_not_of(' ');
		if (cmd_pos == std::string::npos) return;
		size_t cmd_endpos = line.find_first_of(' ', cmd_pos);
		auto str = line.substr(cmd_pos, cmd_endpos);

		// Set or get
		bool is_cmd_set = str == s_Set;
		bool is_cmd_get = str == s_Get;

		if (is_cmd_set || is_cmd_get)
		{
			// Get specialized command and argument.

			// No variable provided.
			if (line.length() == cmd_endpos || cmd_endpos == std::string::npos)
			{
				log(ERROR) << s_ErrorNoVar << endl;
				return;
			}

			// Get command.
			size_t var_endpos = line.find_first_of(' ', cmd_endpos + 1);
			String spec_command_name = line.substr(cmd_pos, var_endpos).data();
			String arg;

			if (is_cmd_set)
			{
				// No argument provided.
				if (var_endpos == std::string::npos)
				{
					log(ERROR) << s_ErrorNoArg << endl;
					return;
				}

				// Get arg.
				size_t arg_pos = line.find_first_not_of(' ', var_endpos);
				if (arg_pos == line.length() || (arg_pos - var_endpos != 1) || arg_pos == std::string::npos)
				{
					log(ERROR) << s_ErrorBadArg << endl;
					return;
				}
				size_t arg_endpos = line.find_first_of(' ', arg_pos + 1);

				// No argument provided.
				if (arg_endpos == std::string::npos && (arg_pos - arg_endpos) == 0)
				{
					log(ERROR) << s_ErrorNoVar << endl;
					return;
				}

				// Get argument.
				arg.m_String = line.substr(arg_pos, arg_endpos);

				// TODO: Check if this should be made optional.
				if (arg_endpos != std::string::npos)
				{
					log(WARN) << s_WarnMoreArgs << endl;
				}
			}

			// Command not found.
			auto command = m_CommandContainer.find(spec_command_name.m_String);
			if (command == m_CommandContainer.end())
			{
				log(ERROR) << s_ErrorSetGetNotFound << endl;
				return;
			}

			// Execute command.
			(*command->second)(arg);
		}
		else
		{
			// Normal Command
			auto command = m_CommandContainer.find(str);

			// Command not found.
			if (command == m_CommandContainer.end())
			{
				log(ERROR) << s_ErrorCmdNotFound << endl;
				return;
			}

			// Rest of the line.
			String arg = cmd_endpos == std::string::npos ? "" : line.substr(cmd_endpos).data();

			// Execute command.
			(*command->second)(arg);
		}
	}
}