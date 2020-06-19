#ifndef CCLI_ITEM_H
#define CCLI_ITEM_H
#pragma once

#include "ccli/ccli_pch.h"
#include "ccli/ccli_api.h"

// TODO: Doxygenize classes.
// TODO: Find better name for CommandData

namespace ccli
{
	static const char endl = '\n';

	/*!
	 * \brief Console item type:
	 * 		Command: Only used for commands.
	 * 		Log: Used to log information inside a command.
	 * 		Warning: Warn client through console.
	 * 		Error: Display error to client through console.
	 * 		Info: Any information wished to display through console.
	 * 		None: Empty console item.
	 */
	enum ItemType
	{
		COMMAND = 0,
		LOG,
		WARNING,
		ERROR,
		INFO,
		NONE
	};

	struct CCLI_API Item
	{
		/*!
		 * \brief Create console item type
		 * \param type Item type to be stored
		 */
		explicit Item(ItemType type = ItemType::LOG);

		/*!
		 * \brief Log data to console item
		 * \param str Append string to item data
		 * \return Self (To allow for fluent logging)
		 */
		Item &operator<<(std::string_view str);

		/*!
		 * \brief Get final/styled string of the item
		 * \return Stylized item string
		 */
		[[nodiscard]] std::string get() const;

		ItemType m_Type;			//!< Console item type
		std::string m_Data;			//!< Item string data
		unsigned int m_TimeStamp;	//!< Record timestamp
	};

#define LOG_BASIC_TYPE_DECL(type) ItemLog& operator<<(type data)

	class CCLI_API ItemLog
	{
	public:

		/*!
		 * \brief Log console item
		 * \param type Type of item to log
		 * \return Self (To allow for fluent logging)
		 */
		ItemLog &log(ItemType type);

		/*!
		 * \brief Get logged console items
		 * \return Console log
		 */
		std::vector<Item> &items();

		/*!
		 * \brief Delete console item log history
		 */
		void clear();

		LOG_BASIC_TYPE_DECL(int);

		LOG_BASIC_TYPE_DECL(long);

		LOG_BASIC_TYPE_DECL(float);

		LOG_BASIC_TYPE_DECL(double);

		LOG_BASIC_TYPE_DECL(long long);

		LOG_BASIC_TYPE_DECL(long double);

		LOG_BASIC_TYPE_DECL(unsigned int);

		LOG_BASIC_TYPE_DECL(unsigned long);

		LOG_BASIC_TYPE_DECL(unsigned long long);

		LOG_BASIC_TYPE_DECL(std::string_view);

		LOG_BASIC_TYPE_DECL(char);

	private:
		std::vector<Item> m_Items;
	};
}

#ifdef CCLI_HEADER_ONLY

#include "ccli/ccli_item.inl"

#endif

#endif
