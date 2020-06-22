#pragma once

#ifndef CCLI_HEADER_ONLY
#include "ccli/ccli_system.h"
#endif

namespace ccli
{
	///////////////////////////////////////////////////////////////////////////
	// Public methods /////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	// Commands-Error-Warning strings.
	static constexpr std::string_view s_Set = "set";
	static constexpr std::string_view s_Get = "get";
	static constexpr std::string_view s_Help = "help";
	static constexpr std::string_view s_ErrorNoVar = "No variable provided";
	static constexpr std::string_view s_ErrorNoArg = "No argument provided";
	static constexpr std::string_view s_ErrorMoreArgs = "More arguments than required were specified";
	static constexpr std::string_view s_ErrorSetGetNotFound = "Command doesn't exist and/or variable is not registered";

	CCLI_INLINE System::System()
	{
		// Register help command.
		registerCommand("help", "Display commands information", [&]()
		{
			for (const auto &tuple : commands())
			{
				// Filter set and get.
				if (tuple.first.size() >= 3 && (tuple.first[0] == 's' || tuple.first[0] == 'g') && tuple.first[1] == 'e' && tuple.first[2] == 't' && tuple.first[3] == ' ')
					continue;

				// Print set and get.
				// TODO: Print set and get.

				// Print the rest of commands
				log() << tuple.second->Help();
			}
		});

		// Register pre-defined commands.
		m_CommandSuggestionTree.insert(s_Set.data());
		m_CommandSuggestionTree.insert(s_Get.data());
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

	CCLI_INLINE void System::runScript(const std::string &script_name)
	{
		// Attempt to find script.
		auto script_pair = m_Scripts.find(script_name);

		// Exit if not found.
		if (script_pair == m_Scripts.end())
		{
			m_CommandData.log(ERROR) << "Script \"" << script_name << "\" not found" << ccli::endl;
			return;
		}

		// About to run script.
		m_CommandData.log(INFO) << "Running \"" << script_name << "\"" << ccli::endl;

		// Load if script is empty.
		if (script_pair->second->data().empty())
		{
			try
			{
				script_pair->second->load();
			}
			catch (ccli::Exception & e)
			{
				log(ERROR) << e.what() << ccli::endl;
			}
		}

		// Run script.
		for (const auto &cmd : script_pair->second->data())
		{
			// Parse command.
			runCommand(cmd);
		}
	}

	CCLI_INLINE void System::registerScript(const std::string &name, const std::string &path)
	{
		// Attempt to find scripts.
		auto script = m_Scripts.find(name);

		// Don't register if script already exists.
		if (script == m_Scripts.end())
		{
			m_Scripts[name] = std::make_unique<Script>(path, true);
			m_VariableSuggestionTree.insert(name);
		}
		else
			throw ccli::Exception("ERROR: Script \"" + name + "\" already registered");
	}

	CCLI_INLINE void System::unregisterCommand(const std::string &cmd_name)
	{
		// Exit if non existent.
		if (cmd_name.empty()) return;

		// Get command.
		auto command_it = m_Commands.find(cmd_name);
		auto help_command_it = m_Commands.find("help " + cmd_name);

		// Erase if found.
		if (command_it != m_Commands.end() && help_command_it != m_Commands.end())
		{
			m_CommandSuggestionTree.remove(cmd_name);
			m_VariableSuggestionTree.remove(cmd_name);

			m_Commands.erase(command_it);
			m_Commands.erase(help_command_it);
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
			m_Commands.erase(s_it);
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
			m_Scripts.erase(it);
		}
	}

	// Getters ////////////////////////////////////////////////////////////////

	CCLI_INLINE AutoComplete &System::cmdAutocomplete()
	{ return m_CommandSuggestionTree; }

	CCLI_INLINE AutoComplete &System::varAutocomplete()
	{ return m_VariableSuggestionTree; }

	CCLI_INLINE CommandHistory &System::history()
	{ return m_CommandHistory; }

	CCLI_INLINE std::vector<Item> &System::items()
	{ return m_CommandData.items(); }

	CCLI_INLINE ItemLog &System::log(ItemType type)
	{ return m_CommandData.log(type); }

	CCLI_INLINE std::unordered_map<std::string, std::unique_ptr<CommandBase>> & System::commands()
	{ return m_Commands; }

	CCLI_INLINE std::unordered_map<std::string, std::unique_ptr<Script>> & System::scripts()
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
		bool is_cmd_help = !(is_cmd_set || is_cmd_get) ? command_name == s_Help : false;

		// Edge case for if user is just runs "help" command
		if (is_cmd_help)
		{
			range = line.NextPoi(line_index);
			if (range.first != line.End())
				command_name += " " + line.m_String.substr(range.first, range.second - range.first);
		}

		// Its a set or get command
		else if (is_cmd_set || is_cmd_get)
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
			// Get the arguments.
			String arguments = line.m_String.substr(range.second, line.m_String.size() - range.first);

			// Execute command.
			auto cmd_out = (*command->second)(arguments);

			// Log output.
			if (cmd_out.m_Type != NONE)
				m_CommandData.items().emplace_back(cmd_out);
		}
	}
}