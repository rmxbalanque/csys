//
// Created by roland on 6/6/20.
//

#ifndef TEQUILA_CCLI_COMMAND_DATA_H
#define TEQUILA_CCLI_COMMAND_DATA_H
#pragma once

#include "base.h"

// TODO: Doxygenize classes.
// TODO: Find better name for CommandData

namespace ccli
{
	static const char endl = '\n';

	enum CCLI_API ItemType
	{
		COMMAND = 0,
		LOG,
		WARN,
		ERROR,
	};

	struct CCLI_API CommandItem
	{
		explicit CommandItem(ItemType type = ItemType::LOG) : m_Type(type)
		{};

		CommandItem &operator<<(std::string_view str);

		[[nodiscard]] std::string get() const;

		ItemType m_Type;
		std::string m_Data;
	};

#define LOG_BASIC_TYPE_DECL(type) CommandData& operator<<(type data);

	class CommandData
	{
	public:

		CommandData &log(ItemType type);

		std::vector<CommandItem> &items();

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

		CommandData &operator<<(std::string_view str);

		CommandData &operator<<(char c);

	private:
		std::vector<CommandItem> m_Items;
	};
}

#ifdef CCLI_HEADER_ONLY

#include "ccli_command_data.inl"

#endif

#endif //TEQUILA_CCLI_COMMAND_DATA_H
