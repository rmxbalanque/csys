#include "doctest.h"
#include "ccli_autocomplete.h"

#define SEARCH_CHECK(word){\
	CHECK(tree.search(word));}\

#define SUGGESTION_CHECK(tree, prefix, ...)\
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

#define SUGGESTION_PARTIAL_CHECK(tree, prefix, partial_ac, ...)\
	{\
		auto suggestions = std::vector<std::string>({__VA_ARGS__});\
		std::vector<std::string> results;\
		std::string partial = prefix;\
		tree.suggestions(partial, results, true);\
		bool check = false;\
		for (const auto &result : results)\
		{\
			for (const auto &suggestion : suggestions)\
			{\
				if (suggestion == result)\
					check = true;\
			}\
		}\
		bool check_2 = partial == partial_ac;\
		CHECK_MESSAGE(check, "Autocomplete suggestions did not match expected output");\
		CHECK_MESSAGE(check_2, std::string("Partial completion did not match expected output -> ") + partial + " != " + partial_ac);\
	}

TEST_CASE("Autocomplete")
{
	ccli::acTernarySearchTree tree({"roland", "munguia", "12345", "michael", "rino", "muchos"});

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
		SUGGESTION_CHECK(tree, "r", "roland", "rino")
		SUGGESTION_CHECK(tree, "ro", "roland")
		SUGGESTION_CHECK(tree, "ri", "rino")
		SUGGESTION_CHECK(tree, "1", "12345")
		SUGGESTION_CHECK(tree, "m", "munguia", "michael")
		SUGGESTION_CHECK(tree, "mu", "munguia", "muchos")
	}

	// Insertion.
	SUBCASE("Inserting + Searching for autocomplete")
	{
		SUGGESTION_CHECK(tree, "r", "roland", "rino")
		SUGGESTION_CHECK(tree, "ro", "roland")
		CHECK(tree.count() == 6);
		CHECK(tree.size() == 31);
		tree.insert("rolling");
		SUGGESTION_CHECK(tree, "r", "roland", "rino", "rolling")
		SUGGESTION_CHECK(tree, "ro", "roland", "rolling")
		CHECK(tree.size() == 35);
		CHECK(tree.count() == 7);
		tree.insert("rolling");
		CHECK(tree.size() == 35);
		CHECK(tree.count() == 7);
	}

	// Partial completion
	ccli::acTernarySearchTree tree2({"roland", "munguia", "12345", "michael", "rolling", "muchos", "rolipoli"});
	SUBCASE("Searching for partial complete + suggestions")
	{
		SUGGESTION_PARTIAL_CHECK(tree2, "r", "rol", "roland", "rolipoli", "rolling")
		SUGGESTION_PARTIAL_CHECK(tree2, "m", "m", "munguia", "muchos", "michael")
	}

	// ADD MORE TESTS
	// Tree duplication.
	// Move operator.
	// Tree deletion.
}

