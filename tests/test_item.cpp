#include "doctest.h"
#include "ccli/ccli_item.h"

TEST_CASE("Autocomplete")
{
	// Testing none.
	SUBCASE("Testing Command Item Types + Basic functionality")
	{
		// LOG
		auto temp = ccli::Item();
		CHECK(temp.m_Type == ccli::ItemType::LOG);
		CHECK(temp.get() == "\t");
		CHECK(temp.m_Data.empty());

		// Command
		temp = ccli::Item(ccli::ItemType::COMMAND);
		CHECK(temp.m_Type == ccli::ItemType::COMMAND);
		CHECK(temp.get() == "> ");
		CHECK(temp.m_Data.empty());

		// Warning
		temp = ccli::Item(ccli::ItemType::WARNING);
		CHECK(temp.m_Type == ccli::ItemType::WARNING);
		CHECK(temp.get() == "\t[WARNING]: ");
		CHECK(temp.m_Data.empty());

		// Error
		temp = ccli::Item(ccli::ItemType::ERROR);
		CHECK(temp.m_Type == ccli::ItemType::ERROR);
		CHECK(temp.get() == "[ERROR]: ");
		CHECK(temp.m_Data.empty());

		// Info
		temp = ccli::Item(ccli::ItemType::INFO) << "Test";
		CHECK(temp.m_Type == ccli::ItemType::INFO);
		CHECK(temp.get() == "Test");

		// None
		temp = ccli::Item(ccli::ItemType::NONE);
		CHECK(temp.m_Type == ccli::ItemType::NONE);
		CHECK(temp.get() == "");
		CHECK(temp.m_Data.empty());
	}

	SUBCASE("Testing Item Log")
	{
		// Logging an item.
		ccli::ItemLog temp;
		temp.log(ccli::LOG);
		CHECK(temp.items().size() == 1);
		temp.clear();
		CHECK(temp.items().size() == 0);
	}
}

