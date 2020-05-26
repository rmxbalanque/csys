//
// Created by roland on 5/25/20.
//

#ifndef CCLI_CCLI_AUTOCOMPLETE_H
#define CCLI_CCLI_AUTOCOMPLETE_H

#pragma once

#include <vector>
#include "base.h"

namespace ccli
{
	// TODO: Check how to add suppport for UTF Encoding.

	// Auto complete ternary tree.
	template<typename charType = char>
	class CCLI_API acTernarySearchTree
	{
	public:

		// Autocomplete node.
		struct acNode
		{
			explicit acNode(const charType data, bool isWord = false) : m_Data(data), m_Less(nullptr), m_Equal(nullptr), m_Greater(nullptr), m_IsWord(isWord)
			{};

			explicit acNode(const charType &&data, bool isWord = false) : m_Data(data), m_Less(nullptr), m_Equal(nullptr), m_Greater(nullptr), m_IsWord(isWord)
			{};

			~acNode()
			{
				delete m_Less;
				delete m_Equal;
				delete m_Greater;
			};

			charType m_Data;
			bool m_IsWord;
			acNode *m_Less;
			acNode *m_Equal;
			acNode *m_Greater;
		};

		// Default constructor.
		acTernarySearchTree() = default;

		// TODO: Deep copy.
		acTernarySearchTree(const acTernarySearchTree &tree) = default;
		acTernarySearchTree(acTernarySearchTree &&tree) noexcept = default;

		template<typename inputType>
		acTernarySearchTree(std::initializer_list<inputType> il)
		{
			for (const auto &item : il)
			{
				insert(item);
			}
		};

		template<typename T>
		explicit acTernarySearchTree(const T &items)
		{
			for (const auto &item : items)
			{
				insert(item);
			}
		};

		// Vector constructor.
		~acTernarySearchTree()
		{
			delete m_Root;
		}

		// Search word.
		template<typename T>
		std::pair<acNode *, bool> search(const char *string)
		{
			acNode *ptr = m_Root;

			// Traverse tree in look for the given string.
			while (ptr)
			{
				if (*string < ptr->m_Data)
				{
					ptr = ptr->m_Less;
				}
				else if (*string == ptr->m_Data)
				{
					// Word was found.
					if (*(string + 1) == '\0' && ptr->m_IsWord)
						return std::make_pair(ptr, true);

					ptr = ptr->m_Equal;
					++string;
				}
				else
				{
					ptr = ptr->m_Greater;
				}
			}

			return std::make_pair(nullptr, false);
		}

		// Character overload.
		void insert(const char *str)
		{
			acNode **ptr = &m_Root;

			while (*str != '\0')
			{
				// Insert char into tree.
				if (*ptr == nullptr)
				{
					*ptr = new acNode(*str);
				}

				// Traverse tree.
				if (*str < (*ptr)->m_Data)
				{
					ptr = &(*ptr)->m_Less;
				}
				else if (*str == (*ptr)->m_Data)
				{
					// String is already in tree, therefore only mark as word.
					if (*(str + 1) == '\0') (*ptr)->m_IsWord = true;

					// Advance.
					ptr = &(*ptr)->m_Equal;
					++str;
				}
				else
				{
					ptr = &(*ptr)->m_Greater;
				}
			}
		}

		// Iterations support.
		template<typename inputType>
		void insert(const inputType & str)
		{
			acNode **ptr = &m_Root;

			while (*str != '\0')
			{
				// Insert char into tree.
				if (*ptr == nullptr)
				{
					*ptr = new acNode(*str);
				}

				// Traverse tree.
				if (*str < (*ptr)->m_Data)
				{
					ptr = &(*ptr)->m_Less;
				}
				else if (*str == (*ptr)->m_Data)
				{
					// String is already in tree, therefore only mark as word.
					if (*(str + 1) == '\0') (*ptr)->m_IsWord = true;

					// Advance.
					ptr = &(*ptr)->m_Equal;
					++str;
				}
				else
				{
					ptr = &(*ptr)->m_Greater;
				}
			}
		}

		// String specialization.
		template<>
		void insert(const std::string & str)
		{
			insert(str.data());
		}

		template<typename strType>
		void suggestions(const strType &prefix, std::vector<std::string> &ac_options)
		{
			acNode *ptr = m_Root;
			auto temp = prefix;

			// Traverse tree and check if prefix exists.
			while (ptr)
			{
				if (*prefix < ptr->m_Data)
				{
					ptr = ptr->m_Less;
				}
				else if (*prefix == ptr->m_Data)
				{
					// Prefix exists in tree.
					if (*(prefix + 1) == '\0')
						break;

					ptr = ptr->m_Equal;
					++prefix;
				}
				else
				{
					ptr = ptr->m_Greater;
				}
			}

			// Already a word. (No need to auto complete).
			if (ptr && ptr->m_IsWord) return;

			// Retrieve auto complete options.
			suggestionsAux(ptr->m_Equal, ac_options, temp);
		}

		void suggestions(const char *prefix, std::vector<std::string> &ac_options)
		{
			acNode *ptr = m_Root;
			auto temp = prefix;

			// Traverse tree and check if prefix exists.
			while (ptr)
			{
				if (*prefix < ptr->m_Data)
				{
					ptr = ptr->m_Less;
				}
				else if (*prefix == ptr->m_Data)
				{
					// Prefix exists in tree.
					if (*(prefix + 1) == '\0')
						break;

					ptr = ptr->m_Equal;
					++prefix;
				}
				else
				{
					ptr = ptr->m_Greater;
				}
			}

			// Already a word. (No need to auto complete).
			if (ptr && ptr->m_IsWord) return;

			// Retrieve auto complete options.
			suggestionsAux(ptr->m_Equal, ac_options, temp);
		}

		template<typename strType>
		std::vector<std::string> * suggestions(const strType &prefix)
		{
			auto temp = new std::vector<std::string>();
			suggestions(prefix, *temp);
			return temp;
		}

		std::vector<std::string> * suggestions(const char * prefix)
		{
			auto temp = new std::vector<std::string>();
			suggestions(prefix, *temp);
			return temp;
		}

	private:

		void suggestionsAux(acNode *root, std::vector<std::string> &ac_options, std::string buffer)
		{
			if (!root) return;

			// Continue looking in left branch.
			if (root->m_Less) suggestionsAux(root->m_Less, ac_options, buffer);

			// Word was found, push into autocomplete options.
			if (root->m_IsWord)
			{
				ac_options.push_back(buffer.append(1, root->m_Data));
				buffer.pop_back();
			}

			// Continue in middle branch, and push character.
			if (root->m_Equal)
			{
				suggestionsAux(root->m_Equal, ac_options, buffer.append(1, root->m_Data));
				buffer.pop_back();
			}

			// Continue looking in right branch.
			if (root->m_Greater)
			{
				suggestionsAux(root->m_Greater, ac_options, buffer);
			}
		}

	private:
		acNode *m_Root = nullptr;
	};
}

#endif //CCLI_CCLI_AUTOCOMPLETE_H
