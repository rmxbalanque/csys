#include <iostream>
#include "doctest.h"
#include "ccli_autocomplete.h"

#define SEARCH_CHECK(word){\
	CHECK(tree.search(word));}\

#define SUGGESTION_CHECK(prefix, ...)\
	{\
		auto suggestions = std::vector<std::string>({__VA_ARGS__});\
		auto results = tree.suggestions(prefix);\
		bool check = false;\
		for (const auto &result : *results)\
		{\
			for (const auto &suggestion : suggestions)\
			{\
				if (suggestion == result)\
					check = true;\
			}\
		}\
		CHECK(check);\
	}\

TEST_CASE("Autocomplete")
{
	ccli::acTernarySearchTree::sVector test = {"roland", "munguia", "12345", "michael", "rino", "muchos"};
	ccli::acTernarySearchTree tree(test);

	// Word search.
	SUBCASE("Searching for word completion")
	{
		SEARCH_CHECK("roland")
		SEARCH_CHECK("munguia")
		SEARCH_CHECK("12345")
		SEARCH_CHECK("michael")
		SEARCH_CHECK("rino")
		SEARCH_CHECK("muchos")
	}

	// Completion.
	SUBCASE("Searching for auto complete")
	{
		SUGGESTION_CHECK("r", "roland", "rino")
		SUGGESTION_CHECK("ro", "roland")
		SUGGESTION_CHECK("ri", "rino")
		SUGGESTION_CHECK("1", "12345")
		SUGGESTION_CHECK("m", "munguia", "michael")
		SUGGESTION_CHECK("mu", "munguia", "muchos")
	}

	// ADD MORE TESTS
	// Tree duplication.
	// Move operator.
	// Tree deletion.
}

