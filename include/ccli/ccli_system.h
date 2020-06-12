#ifndef CCLI_SYSTEM_H
#define CCLI_SYSTEM_H

#pragma once

#include "ccli_pch.h"
#include "ccli_command.h"
#include "ccli_autocomplete.h"
#include "ccli_history.h"
#include "ccli_command_data.h"

namespace ccli
{
	// TODO: Add a CommandData Into Registration and return it when parsing.
	// TODO: Use modern pointers.

	class CCLI_API System
	{
	public:

		/*!
		 * \brief Initialize system object
		 */
		System();

		/*!
		 * \brief Parse given command line
		 * \param line Command line string
		 */
		void parse(const std::string &line);

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
		 * \brief Get registered command container
		 * \return Commands container
		 */
		std::unordered_map<std::string, CommandBase *> commands();

		template<typename Fn, typename ...Args>
		void registerCommand(const String &name, const String &description, Fn function, Args... args)
		{
			// Check if function can be called with the given arguments
			static_assert(std::is_invocable_v<Fn, typename Args::ValueType...>, "Arguments specified do not match that of the function");

			// TODO: This should be an exception.
			if (m_CommandContainer.find(name.m_String) != m_CommandContainer.end())
			{
				std::cout << "ERROR: Command already exists." << std::endl;
				return;
			}

			// Register for autocomplete.
			if (m_RegisterCommandSuggestion)
			{
				m_SuggestionTree.insert(name.m_String);
			}

			// Add commands to system here
			m_CommandContainer[name.m_String] = new Command<Fn, Args...>(name, description, function, args...);
		}

		template<typename T, typename V>
		void registerVariable(const std::string &name, V &var)
		{
			// Disable.
			m_RegisterCommandSuggestion = false;

			// Register set.
			ccli::System::registerCommand(std::string("set " + name).data(), "Set variable", [&var](T value)
			{ var = value; }, ccli::Arg<T>(name.data()));

			// Register get.
			ccli::System::registerCommand(std::string("get " + name).data(), "Get variable", [&]()
			{ m_CommandData.log(LOG) << var << endl; });

			// Enable again.
			m_RegisterCommandSuggestion = true;

			// Register variable
			m_VariableSuggestionTree.insert(name.c_str());
		}

	private:
		std::unordered_map<std::string, CommandBase *> m_CommandContainer;	//!< Registered command container
		acTernarySearchTree m_SuggestionTree;								//!< Autocomplete Ternary Search Tree for commands
		acTernarySearchTree m_VariableSuggestionTree;						//!< Autocomplete Ternary Search Tree for registered variables
		CommandHistory m_CommandHistory;									//!< History of executed commands
		CommandData m_CommandData;											//!< Console Items (Logging)

		bool m_RegisterCommandSuggestion = true;							//!< Flag that determines if commands will be registered for autcoomplete.
	};
}

#ifdef CCLI_HEADER_ONLY

#include "ccli_system.inl"

#endif

#endif //CCLI_SYSTEM_H
