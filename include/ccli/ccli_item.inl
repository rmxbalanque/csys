
#pragma once

#ifndef CCLI_HEADER_ONLY
#include "ccli/ccli_item.h"
#endif


namespace ccli
{

	///////////////////////////////////////////////////////////////////////////
	// Command Item ///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	CCLI_INLINE static constexpr std::string_view s_Command = "> ";
	CCLI_INLINE static constexpr std::string_view s_Warning = "\t[WARNING]: ";
	CCLI_INLINE static constexpr std::string_view s_Error = "[ERROR]: ";
	CCLI_INLINE static const auto s_TimeBegin = std::chrono::steady_clock::now();

	CCLI_INLINE Item::Item(ItemType type) : m_Type(type)
	{
		auto timeNow = std::chrono::steady_clock::now();
		m_TimeStamp = static_cast<unsigned int>(std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - s_TimeBegin).count());
	}

	CCLI_INLINE Item &Item::operator<<(const std::string_view str)
	{
		m_Data.append(str);
		return *this;
	}

	CCLI_INLINE std::string Item::get() const
	{
		switch (m_Type)
		{
			case COMMAND:
				return s_Command.data() + m_Data;
			case LOG:
				return '\t' + m_Data;
			case WARNING:
				return s_Warning.data() + m_Data;
			case ERROR:
				return s_Error.data() + m_Data;
			case INFO:
				return m_Data;
			case NONE:
			default:
				return "";
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// Command Data ///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

#define LOG_BASIC_TYPE_DEF(type)\
    CCLI_INLINE ItemLog& ItemLog::operator<<(type data)\
    {\
        m_Items.back() << std::to_string(data);\
        return *this;\
    }

	CCLI_INLINE ItemLog &ItemLog::log(ItemType type)
	{
		// New item.
		m_Items.emplace_back(type);
		return *this;
	}

	CCLI_INLINE std::vector<Item> &ItemLog::items()
	{
		return m_Items;
	}

	CCLI_INLINE void ItemLog::clear()
	{
		m_Items.clear();
	}

	CCLI_INLINE ItemLog &ItemLog::operator<<(const std::string_view data)
	{
		m_Items.back() << data;
		return *this;
	}

	CCLI_INLINE ItemLog &ItemLog::operator<<(const char data)
	{
		m_Items.back().m_Data.append(1, data);
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