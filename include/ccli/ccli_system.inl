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
	static constexpr std::string_view s_ErrorBadArg = "Not a valid argument";
	static constexpr std::string_view s_ErrorNoArg = "No argument provided";
	static constexpr std::string_view s_ErrorMoreArgs = "More arguments than required were specified";
	static constexpr std::string_view s_ErrorSetGetNotFound = "Command doesn't exist and/or variable is not registered";
	static constexpr std::string_view s_ErrorCmdNotFound = "Command doesn't exist";

	CCLI_INLINE System::System()
	{
		// Register pre-defined commands.
		m_CommandSuggestionTree.insert(s_Set.data());
		m_CommandSuggestionTree.insert(s_Get.data());
	}

	CCLI_INLINE System::~System()
	{
		// Erase commands.
		for (const auto & cmd : m_Commands)
			delete cmd.second;

		// Erase scripts.
		for (const auto & script : m_Scripts)
			delete script.second;
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

	CCLI_INLINE void System::registerScript(const std::string &name, std::filesystem::path path)
	{
		// Helper for easy file accessing.
		// TODO: Check what to do with this.
		if (path == std::filesystem::current_path())
			path /= name;

		// Attempt to find scripts.
		auto script = m_Scripts.find(name);

		// TODO: Assert or cout error? Or other.
		// Don't register if script already exists.
		if (script == m_Scripts.end())
			m_Scripts[name] = new Script(path.string(), true);

		m_VariableSuggestionTree.insert(name);
	}

	CCLI_INLINE void System::unregisterCommand(const std::string &cmd_name)
	{
		// Exit if non existent.
		if (cmd_name.empty()) return;

		// Get command.
		auto it = m_Commands.find(cmd_name);

		// Erase if found.
		if (it != m_Commands.end())
		{
			m_CommandSuggestionTree.remove(cmd_name);
			delete it->second;
			m_Commands.erase(it);
		}
	}

	CCLI_INLINE void System::unregisterVariable(const std::string &var_name)
	{
		// Exit if non existent.
		if (var_name.empty()) return;

		// Get command.
		auto s_it = m_Commands.find("set " + var_name);
		auto g_it = m_Commands.find("get " + var_name);

		// Erase if found.
		if (s_it != m_Commands.end() && g_it != m_Commands.end())
		{
			m_VariableSuggestionTree.remove(var_name);

			delete s_it->second;
			m_Commands.erase(s_it);

			delete g_it->second;
			m_Commands.erase(g_it);
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
			m_VariableSuggestionTree.remove(script_name);
			delete it->second;
			m_Scripts.erase(it);
		}
	}

	// Getters ////////////////////////////////////////////////////////////////

	CCLI_INLINE acTernarySearchTree &System::cmdAutocomplete()
	{ return m_CommandSuggestionTree; }

	CCLI_INLINE acTernarySearchTree &System::varAutocomplete()
	{ return m_VariableSuggestionTree; }

	CCLI_INLINE CommandHistory &System::history()
	{ return m_CommandHistory; }

	CCLI_INLINE std::vector<CommandItem> &System::items()
	{ return m_CommandData.items(); }

	CCLI_INLINE CommandData &System::log(ItemType type)
	{ return m_CommandData.log(type); }

	CCLI_INLINE std::unordered_map<std::string, CommandBase *> System::commands()
	{ return m_Commands; }

	CCLI_INLINE std::unordered_map<std::string, Script *> System::scripts()
	{ return m_Scripts; }

	///////////////////////////////////////////////////////////////////////////
	// Private methods ////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	CCLI_INLINE void System::parseCommandLine(const String &line)
	{
		// Get first non-whitespace char.
		size_t line_index = 0;
		std::pair<size_t, size_t> range = line.NextPoi(line_index);

		// Just whitespace was passed in. Don't log as command.
		if (range.first == line.End())
			return;

		// Push to history.
		m_CommandHistory.push_back(line.m_String);

		// Get name of command.
		std::string command_name = line.m_String.substr(range.first, range.second - range.first);

		// Set or get
		bool is_cmd_set = command_name == s_Set;
		bool is_cmd_get = command_name == s_Get;

		// Its a set or get command
		if (is_cmd_set || is_cmd_get)
		{
			// Try to get variable name
			if ((range = line.NextPoi(line_index)).first == line.End())
			{
				log(ERROR) << s_ErrorNoVar << endl;
				return;
			}
			else
				// Append variable name.
				command_name += " " + line.m_String.substr(range.first, range.second - range.first);
		}

		// Get runnable command
		auto command = m_Commands.find(command_name);
		if (command == m_Commands.end())
			log(ERROR) << s_ErrorSetGetNotFound << endl;
		// Run the command
		else
		{
			// If the command takes arguments and none were provided
			if (command->second->TakesArguments() && (range = line.NextPoi(line_index)).first == line.End())
			{
				log(ERROR) << s_ErrorNoArg << endl;
				return;
			}

			// If its the set command, check if only one thing is passed in
			if (is_cmd_set && line.m_String[range.first] != '"' && line.NextPoi(line_index).first != line.End())
			{
				log(ERROR) << s_ErrorMoreArgs << endl;
				return;
			}

			// Get the arguments.
			String arguments = line.m_String.substr(range.first, line.m_String.size());

			// Execute command.
			auto cmd_out = (*command->second)(arguments);

			// Log output.
			if (cmd_out.m_Type != NONE)
				m_CommandData.items().emplace_back(cmd_out);
		}
	}
}