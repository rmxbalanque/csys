
#pragma once

#ifndef CCLI_HEADER_ONLY
#include "ccli_command_data.h"
#endif

namespace ccli
{

	///////////////////////////////////////////////////////////////////////////
	// Command Item ///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	static constexpr std::string_view s_Command = "> ";
	static constexpr std::string_view s_Warning = "[WARNING]: ";
	static constexpr std::string_view s_Error = "[ERROR]: ";

	CCLI_INLINE CommandItem &CommandItem::operator<<(const std::string_view str)
	{
		m_Data.append(str);
		return *this;
	}

	CCLI_INLINE std::string CommandItem::get() const
	{
		switch (m_Type)
		{
			case ItemType::COMMAND:
				return s_Command.data() + m_Data;
			case ItemType::LOG:
				return '\t' + m_Data;
			case ItemType::WARN:
				return '\t' + (s_Warning.data() + m_Data);
			case ItemType::ERROR:
				return s_Error.data() + m_Data;
			default:
				return "";
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// Command Data ///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

#define LOG_BASIC_TYPE_DEF(type)\
    CCLI_INLINE CommandData& CommandData::operator<<(type data)\
    {\
        m_Items.back() << std::to_string(data);\
        return *this;\
    }

	CCLI_INLINE CommandData &CommandData::log(ItemType type)
	{
		// New item.
		m_Items.emplace_back(type);
		return *this;
	}

	CCLI_INLINE std::vector<CommandItem> &CommandData::items()
	{
		return m_Items;
	}

	CCLI_INLINE void CommandData::clear()
	{
		m_Items.clear();
	}

	CCLI_INLINE CommandData &CommandData::operator<<(const std::string_view str)
	{
		m_Items.back() << str;
		return *this;
	}

	CCLI_INLINE CommandData &CommandData::operator<<(const char c)
	{
		m_Items.back().m_Data.append(1, c);
		return *this;
	}

	// Basic type operator definitions.
	LOG_BASIC_TYPE_DEF(int)

	LOG_BASIC_TYPE_DEF(long)

	LOG_BASIC_TYPE_DEF(float)

	LOG_BASIC_TYPE_DEF(double)

	LOG_BASIC_TYPE_DEF(long long)

	LOG_BASIC_TYPE_DEF(long double)

	LOG_BASIC_TYPE_DEF(unsigned int)

	LOG_BASIC_TYPE_DEF(unsigned long)

	LOG_BASIC_TYPE_DEF(unsigned long long)
}