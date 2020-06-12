#include "doctest.h"
#include "ccli_history.h"

TEST_CASE("Test ccli history ")
{
	// Check insertion.
	SUBCASE("Testing command history insertion")
	{
		ccli::CommandHistory c_history;
		std::vector<std::string> commands({"h1", "h2", "h3", "h4"});

		for (const auto & cmd : commands)
		{
			c_history.push_back(cmd);
		}

		for (int i = 0; i < commands.size(); ++i)
		{
			CHECK(c_history[i] == commands[i]);
		}

		CHECK(c_history.get_new() == commands.back());
	}

	SUBCASE("Check history looping")
	{
		ccli::CommandHistory c_history(3);
		std::vector<std::string> commands({"h1", "h2", "h3", "h4"});

		for (const auto & cmd : commands)
		{
			c_history.push_back(cmd);
			CHECK(c_history.get_new() == cmd);
		}

		CHECK(c_history[0] == commands[3]);
		CHECK(c_history[1] == commands[1]);
		CHECK(c_history[2] == commands[2]);

		CHECK(c_history.get_old() == "h2");
		CHECK(c_history.get_old_index() ==  1);
		CHECK(c_history.get_new() == commands.back());
		CHECK(c_history.size() == 3);

		c_history.clear();
		for (int i = 0; i < 3; ++i)
			c_history.push_back(commands[i]);
		CHECK(c_history.get_old() == "h1");
		CHECK(c_history.get_old_index() == 0);
		CHECK(c_history.get_new() == "h3");
	}
	// Check looping.

	// Check clearing.
	SUBCASE("CHECK history clearing")
	{
		ccli::CommandHistory c_history(64);
		std::vector<std::string> commands({"h1", "h2", "h3", "h4"});

		for (const auto & cmd : commands)
		{
			c_history.push_back(cmd);
		}

		CHECK(c_history.size() == 4);

		for (int i = 0; i < c_history.capacity(); ++i)
			c_history.push_back("dummy" + std::to_string(i));

		CHECK(c_history.size() == c_history.capacity());

		CHECK(c_history.get_new() == "dummy63");
		CHECK(c_history.get_new_index() == 3);
		CHECK(c_history.get_old_index() == 4);
		CHECK(c_history.index(-1) == "dummy60");
		CHECK(c_history.index(90000) == "dummy59");

		c_history.clear();
		CHECK(c_history.size() == 0);
	}
}