#include "doctest.h"
#include "ccli_command_data.h"

TEST_CASE("Autocomplete")
{
	// Testing none.
	SUBCASE("Testing Command Item Types + Basic functionality")
	{
		// LOG
		auto temp = ccli::CommandItem();
		CHECK(temp.m_Type == ccli::ItemType::LOG);
		CHECK(temp.get() == "\t");
		CHECK(temp.m_Data.empty());

		// Command
		temp = ccli::CommandItem(ccli::ItemType::COMMAND);
		CHECK(temp.m_Type == ccli::ItemType::COMMAND);
		CHECK(temp.get() == "> ");
		CHECK(temp.m_Data.empty());

		// Warning
		temp = ccli::CommandItem(ccli::ItemType::WARNING);
		CHECK(temp.m_Type == ccli::ItemType::WARNING);
		CHECK(temp.get() == "\t[WARNING]: ");
		CHECK(temp.m_Data.empty());

		// Error
		temp = ccli::CommandItem(ccli::ItemType::ERROR);
		CHECK(temp.m_Type == ccli::ItemType::ERROR);
		CHECK(temp.get() == "[ERROR]: ");
		CHECK(temp.m_Data.empty());

		// None
		temp = ccli::CommandItem(ccli::ItemType::NONE);
		CHECK(temp.m_Type == ccli::ItemType::NONE);
		CHECK(temp.get() == "");
		CHECK(temp.m_Data.empty());
	}

	SUBCASE("Testing Command Data")
	{
		ccli::CommandData temp;
		temp.log(ccli::LOG);
		CHECK(temp.items().size() == 1);
		temp.clear();
		CHECK(temp.items().size() == 0);
	}
}

