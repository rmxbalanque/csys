#ifndef CCLI_SYSTEM_H
#define CCLI_SYSTEM_H

#pragma once

// TODO: Forward declare as much as possible
#include "ccli/ccli_command.h"
#include "ccli/ccli_autocomplete.h"
#include "ccli/ccli_history.h"
#include "ccli/ccli_item.h"
#include "ccli/ccli_script.h"
#include <memory>
#include <unordered_map>
#include <string>

namespace ccli
{
	// TODO: Add move, copy, assignment operators to all classes in ccli. Or delete if thats the intended purpose.
	// TODO: Trim white space for the string both ends.
	class CCLI_API System
	{
	public:

		/*!
		 * \brief Initialize system object
		 */
		System();

		/*!
		 * \brief Parse given command and run it
		 * \param line Command line string
		 */
		void runCommand(const std::string &line);

		/*!
		 * \brief Get console registered command autocomplete tree
		 * \return Autocomplete Ternary Search Tree
		 */
		AutoComplete &cmdAutocomplete();

		/*!
		 * \brief Get console registered variables autocomplete tree
		 * \return Autocomplete Ternary Search Tree
		 */
		AutoComplete &varAutocomplete();

		/*!
		 * \brief Get command history container
		 * \return Command history vector
		 */
		CommandHistory &history();

		/*!
		 * \brief Get console items
		 * \return Console items container
		 */
		std::vector<Item> &items();

		/*!
		 * \brief Creates a new item entry to log information
		 * \param type Log type (COMMAND, LOG, WARNING, ERROR)
		 * \return Reference to console items obj
		 */
		ItemLog &log(ItemType type = ItemType::LOG);

		/*!
		 * \brief Run the given script
		 * \param script_name Script to be executed
		 *
		 *  \note If script exists but its not loaded, this methods will load the script and proceed to run it.
		 */
		void runScript(const std::string &script_name);

		/*!
		 * \brief Get registered command container
		 * \return Commands container
		 */
		std::unordered_map<std::string, std::unique_ptr<CommandBase>> & commands();

		/*!
		 * \brief Get registered scripts container
		 * \return Scripts container
		 */
		std::unordered_map<std::string, std::unique_ptr<Script>> & scripts();

		template<typename Fn, typename ...Args>
		void registerCommand(const String &name, const String &description, Fn function, Args... args)
		{
			// Check if function can be called with the given arguments
			static_assert(std::is_invocable_v<Fn, typename Args::ValueType...>, "Arguments specified do not match that of the function");
			static_assert(!std::is_member_function_pointer_v<Fn>, "Non-static member functions are not allowed");

			size_t name_index = 0;
			auto range = name.NextPoi(name_index);

			// Error out.
			if (m_Commands.find(name.m_String) != m_Commands.end())
				throw ccli::Exception("ERROR: Command already exists");
			// Check if command is empty
			else if (range.first == name.End())
			{
				log(ERROR) << "Empty command given" << ccli::endl;
				return;
			}

			// Get command name
			std::string command_name = name.m_String.substr(range.first, range.second - range.first);

			// Command is not help and contains more than one word
			if (name.NextPoi(name_index).first != name.End())
				throw ccli::Exception("ERROR: Command names can not compose of multiple words");

			// Register for autocomplete.
			if (m_RegisterCommandSuggestion)
			{
				m_CommandSuggestionTree.insert(command_name);
				m_VariableSuggestionTree.insert(command_name); // TODO: Ask roland if I need to add just the name or the help too
			}

			// Add commands to system here
			m_Commands[name.m_String] = std::make_unique<Command<Fn, Args...>>(name, description, function, args...);

			// Set Help
			auto help = [this, command_name]() { log(LOG) << m_Commands[command_name]->Help() << ccli::endl; };
			m_Commands["help " + command_name] = std::make_unique<Command<decltype(help)>>("help " + command_name,
																																			 "Displays help info about command " + command_name,
																													 						 help);
		}

		template<typename T>
		void registerVariable(const String &name, T &var)
		{
			// Disable.
			m_RegisterCommandSuggestion = false;

			// Make sure only one word was passed in
			size_t name_index = 0;
			auto range = name.NextPoi(name_index);
			if (name.NextPoi(name_index).first != name.End())
				throw ccli::Exception("ERROR: Whitespace separated variable names are forbidden");

			// Register set.
			std::string var_name = name.m_String.substr(range.first, range.second - range.first);

			// TODO: Tell roland to avoid doing & in lambdas
			// TODO: Write in documentation that the memory passed into this function is assumed with the lifetime of the program
			const auto SetFunction = [&var](T val){ var = val; };
			const auto GetFunction = [this, &var](){ m_CommandData.log(LOG) << var << endl; };

			m_Commands["set " + var_name] = std::make_unique<Command<decltype(SetFunction), Arg<T>>>("set " + var_name, "Sets the variable " + var_name, SetFunction, Arg<T>(var_name));
			m_Commands["get " + var_name] = std::make_unique<Command<decltype(GetFunction)>>("get " + var_name, "Gets the variable " + var_name, GetFunction);

			// Enable again.
			m_RegisterCommandSuggestion = true;

			// Register variable
			m_VariableSuggestionTree.insert(var_name); // TODO: Ask roland if I need to add the "set name" and "get name"
		}

		/*!
		 * \brief Register script into console system
		 * \param name Script name
		 * \param path Scrip path
		 */
		void registerScript(const std::string &name, const std::string &path);

		/*!
		 * \brief Unregister command from console system
		 * \param cmd_name Command to unregister
		 */
		void unregisterCommand(const std::string &cmd_name);

		/*!
		 * \brief Unregister variable from console system
		 * \param var_name Variable to unregister
		 */
		void unregisterVariable(const std::string &var_name);

		/*!
		 * \brief Unregister script from console system
		 * \param script_name Script to unregister
		 */
		void unregisterScript(const std::string &script_name);

	private:

		void parseCommandLine(const String & line);											//!< Parse command line and execute command

		std::unordered_map<std::string, std::unique_ptr<CommandBase>> m_Commands;			//!< Registered command container
		AutoComplete m_CommandSuggestionTree;										//!< Autocomplete Ternary Search Tree for commands
		AutoComplete m_VariableSuggestionTree;										//!< Autocomplete Ternary Search Tree for registered variables
		CommandHistory m_CommandHistory;													//!< History of executed commands
		ItemLog m_CommandData;																//!< Console Items (Logging)
		std::unordered_map<std::string, std::unique_ptr<Script>> m_Scripts;					//!< Scripts
		bool m_RegisterCommandSuggestion = true;											//!< Flag that determines if commands will be registered for autocomplete.
	};
}

#ifdef CCLI_HEADER_ONLY
#include "ccli/ccli_system.inl"
#endif

#endif
