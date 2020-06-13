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

	CCLI_INLINE System::System()
	{
		// Register pre-defined commands.
		m_SuggestionTree.insert(s_Set.data());
		m_SuggestionTree.insert(s_Get.data());
	}

	CCLI_INLINE void System::runCommand(const std::string &line)
	{
		// Error checking.
		if (line.empty())
			return;
		
		// Log command.
		log(ccli::ItemType::COMMAND) << line << ccli::endl;

		// Parse command line.
		parseCommandLine(line);
		
	}

	CCLI_INLINE void System::runScript(std::string_view script_name)
	{
		// Attempt to find script.
		auto script_pair = m_Scripts.find(script_name.data());

		// Exit if not found.
		if (script_pair == m_Scripts.end())
		{
			m_CommandData.log(ERROR) << "Script \"" << script_name << "\" not found" << ccli::endl;
			return;
		}

		// About to run script.
		m_CommandData.log(INFO) << "Running \"" << script_name << "\"" << ccli::endl;

		// Run script.
		for (const auto &cmd : script_pair->second->data())
		{
			// Parse command.
			runCommand(cmd);
		}
	}

	CCLI_INLINE void System::registerScript(std::string_view name, std::filesystem::path path)
	{
		// Helper for easy file accessing.
		// TODO: Check what to do with this.
		if (path == std::filesystem::current_path())
			path /= name.data();

		// Attempt to find scripts.
		auto script = m_Scripts.find(name.data());

		// TODO: Assert or cout error? Or other.
		// Don't register if script already exists.
		if (script == m_Scripts.end())
			m_Scripts[name.data()] = new Script(path.c_str());

		// TODO: Register in autocomplete
	}


	CCLI_INLINE void System::unregisterCommand(const std::string &cmd_name)
	{
		// Exit if non existent.
		if (cmd_name.empty()) return;

		// Get command.
		auto it = m_CommandContainer.find(cmd_name);

		// Erase if found.
		if (it != m_CommandContainer.end())
		{
			// TODO: Erase from autocomplete
			delete it->second;
			m_CommandContainer.erase(it);
		}
	}

	CCLI_INLINE void System::unregisterVariable(const std::string &var_name)
	{
		// Exit if non existent.
		if (var_name.empty()) return;

		// Get command.
		auto s_it = m_CommandContainer.find("set " + var_name);
		auto g_it = m_CommandContainer.find("get " + var_name);

		// Erase if found.
		if (s_it != m_CommandContainer.end() && g_it != m_CommandContainer.end())
		{
			// TODO: Erase from autocomplete
			delete s_it->second;
			m_CommandContainer.erase(s_it);

			delete g_it->second;
			m_CommandContainer.erase(g_it);
		}
	}

	CCLI_INLINE void System::unregisterScript(const std::string &script_name)
	{
		// Exit if non existent.
		if (script_name.empty()) return;

		// Get command.
		auto it = m_Scripts.find(script_name);

		// Erase if found.
		if (it != m_Scripts.end())
		{
			// TODO: Erase from autocomplete
			delete it->second;
			m_Scripts.erase(it);
		}
	}

	// Getters ////////////////////////////////////////////////////////////////

	CCLI_INLINE acTernarySearchTree &System::cmdAutocomplete()
	{ return m_SuggestionTree; }

	CCLI_INLINE acTernarySearchTree &System::varAutocomplete()
	{ return m_VariableSuggestionTree; }

	CCLI_INLINE CommandHistory &System::history()
	{ return m_CommandHistory; }

	CCLI_INLINE std::vector<CommandItem> &System::items()
	{ return m_CommandData.items(); }

	CCLI_INLINE CommandData &System::log(ItemType type)
	{ return m_CommandData.log(type); }

	CCLI_INLINE std::unordered_map<std::string, CommandBase *> System::commands()
	{ return m_CommandContainer; }

	CCLI_INLINE std::unordered_map<std::string, Script *> System::scripts()
	{ return m_Scripts; }

	///////////////////////////////////////////////////////////////////////////
	// Private methods ////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	CCLI_INLINE void System::parseCommandLine(const std::string &line)
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
		auto str = line.substr(cmd_pos, cmd_endpos - cmd_pos);

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
			String spec_command_name = line.substr(cmd_pos, var_endpos - cmd_pos).data();
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
				arg.m_String = line.substr(arg_pos, arg_endpos - arg_pos);

				// TODO: Check if this should be made optional.
				if (arg_endpos != std::string::npos)
				{
					log(WARNING) << s_WarnMoreArgs << endl;
				}
			}
			else
			{
				// Get arg.
				size_t arg_pos = line.find_first_not_of(' ', var_endpos);
				if (arg_pos == line.length() || arg_pos != std::string::npos)
				{
					log(WARNING) << s_WarnMoreArgs << endl;
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
			auto cmd_out = (*command->second)(arg);
			
			// Log output.
			if (cmd_out.m_Type != NONE)
				m_CommandData.items().emplace_back(cmd_out);
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
			auto cmd_out = (*command->second)(arg);

			// Log output.
			if (cmd_out.m_Type != NONE)
				m_CommandData.items().emplace_back(cmd_out);
		}
	}
}