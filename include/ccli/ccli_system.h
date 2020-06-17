#ifndef CCLI_SYSTEM_H
#define CCLI_SYSTEM_H

#pragma once

#include "ccli_pch.h"
#include "ccli_command.h"
#include "ccli_autocomplete.h"
#include "ccli_history.h"
#include "ccli_command_data.h"
#include "ccli_script.h"

namespace ccli
{
	// TODO: Use modern pointers.
	// TODO: Add move, copy, assignment operators to all classes in ccli. Or delete if thats the intended purpose.
	// TODO: Trim white space for the string both ends.
	// TODO: SYSTEM EXCEPTIONS AND CODE COMMENTS.
	// TODO: CMakeList Install
	class CCLI_API System
	{
	public:

		/*!
		 * \brief Initialize system object
		 */
		System();

		/*!
		 * \brief Clean up console system object
		 */
		~System();

		/*!
		 * \brief Parse given command and run it
		 * \param line Command line string
		 */
		void runCommand(const std::string &line);

		/*!
		 * \brief Get console registered command autocomplete tree
		 * \return Autocomplete Ternary Search Tree
		 */
		acTernarySearchTree &cmdAutocomplete();

		/*!
		 * \brief Get console registered variables autocomplete tree
		 * \return Autocomplete Ternary Search Tree
		 */
		acTernarySearchTree &varAutocomplete();

		/*!
		 * \brief Get command history container
		 * \return Command history vector
		 */
		CommandHistory &history();

		/*!
		 * \brief Get console items
		 * \return Console items container
		 */
		std::vector<CommandItem> &items();

		/*!
		 * \brief Creates a new item entry to log information
		 * \param type Log type (COMMAND, LOG, WARNING, ERROR)
		 * \return Reference to console items obj
		 */
		CommandData &log(ItemType type = ItemType::LOG);

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
		std::unordered_map<std::string, CommandBase *> commands();

		/*!
		 * \brief Get registered scripts container
		 * \return Scripts container
		 */
		std::unordered_map<std::string, Script *> scripts();

		template<typename Fn, typename ...Args>
		void registerCommand(const String &name, const String &description, Fn function, Args... args)
		{
			// Check if function can be called with the given arguments
			static_assert(std::is_invocable_v<Fn, typename Args::ValueType...>, "Arguments specified do not match that of the function");

			// Error out.
			if (m_Commands.find(name.m_String) != m_Commands.end())
				throw ccli::Exception("ERROR: Command already exists");

			// Register for autocomplete.
			if (m_RegisterCommandSuggestion)
				m_CommandSuggestionTree.insert(name.m_String);

			// Add commands to system here
			m_Commands[name.m_String] = new Command<Fn, Args...>(name, description, function, args...);
		}

		template<typename T>
		void registerVariable(const String &name, T &var)
		{
			// Disable.
			m_RegisterCommandSuggestion = false;

			// Make sure only one word was passed in
			size_t name_index = 0;
			name.NextPoi(name_index);
			auto range = name.NextPoi(name_index);
			if (range.first != name.End())
			{
				throw ccli::Exception("ERROR: Whitespace separated variable names are forbidden");
			}

			// Register set.
			ccli::System::registerCommand("set " + name.m_String, "Sets the variable " + name.m_String, [&var](T value)
			{ var = value; }, ccli::Arg<T>(name));

			// Register get.
			ccli::System::registerCommand("get " + name.m_String, "Gets the variable " + name.m_String, [&]()
			{ m_CommandData.log(LOG) << var << endl; });

			// Enable again.
			m_RegisterCommandSuggestion = true;

			// Register variable
			m_VariableSuggestionTree.insert(name.m_String);
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

		void parseCommandLine(const String & line);							//!< Parse command line and execute command

		std::unordered_map<std::string, CommandBase *> m_Commands;			//!< Registered command container
		acTernarySearchTree m_CommandSuggestionTree;						//!< Autocomplete Ternary Search Tree for commands
		acTernarySearchTree m_VariableSuggestionTree;						//!< Autocomplete Ternary Search Tree for registered variables
		CommandHistory m_CommandHistory;									//!< History of executed commands
		CommandData m_CommandData;											//!< Console Items (Logging)
		std::unordered_map<std::string, Script*> m_Scripts;					//!< Scripts
		bool m_RegisterCommandSuggestion = true;							//!< Flag that determines if commands will be registered for autocomplete.
	};
}

#ifdef CCLI_HEADER_ONLY

#include "ccli_system.inl"

#endif

#endif //CCLI_SYSTEM_H
