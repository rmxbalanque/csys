//
// Created by roland on 5/25/20.
//

#ifndef CCLI_CCLI_AUTOCOMPLETE_H
#define CCLI_CCLI_AUTOCOMPLETE_H

#pragma once

#include <ostream>
#include <vector>
#include "base.h"

namespace ccli
{
	// TODO: UTF8 and UTF16 support.

	// Auto complete ternary tree.
	class acTernaryTree
	{
	public:
		acTernaryTree() = default;

		// Autocomplete node.
		typedef struct acNode
		{
			explicit acNode(const char data, bool isWord = false) : m_Data(data), m_Less(nullptr), m_Equal(nullptr), m_Greater(nullptr), m_IsWord(isWord)
			{};

			explicit acNode(const char &&data, bool isWord = false) : m_Data(data), m_Less(nullptr), m_Equal(nullptr), m_Greater(nullptr), m_IsWord(isWord)
			{};

			char m_Data;
			bool m_IsWord;
			acNode *m_Less;
			acNode *m_Equal;
			acNode *m_Greater;
		} acNode;

		// Result.
		bool search(const char *string)
		{
			acNode *ptr = m_Root;

			while (ptr)
			{
				if (*string < ptr->m_Data)
				{
					ptr = ptr->m_Less;
				}
				else if (*string == ptr->m_Data)
				{
					if (*(string + 1) == '\0' && ptr->m_IsWord)
						return true;

					ptr = ptr->m_Equal;
					++string;
				}
				else
				{
					ptr = ptr->m_Greater;
				}
			}

			return false;
		}

		void insert(const char *str)
		{
			acNode **ptr = &m_Root;

			while (*str != '\0')
			{
				if (*ptr == nullptr)
				{
					//if (*(str + 1) == '\0')
					//	*ptr = new acNode(*str, true);
					//else
						*ptr = new acNode(*str);
				}

				if (*str < (*ptr)->m_Data)
				{
					ptr = &(*ptr)->m_Less;
				}
				else if (*str == (*ptr)->m_Data)
				{
					if (*(str + 1) == '\0') (*ptr)->m_IsWord = true;

					ptr = &(*ptr)->m_Equal;
					++str;
				}
				else
				{
					ptr = &(*ptr)->m_Greater;
				}
			}

			//if (ptr && *ptr) (*ptr)->m_IsWord = true;
		}

		void traverseUtil(std::ostream &os, acNode *root) const
		{
			if (root)
			{
				traverseUtil(os, root->m_Less);
				os << root->m_Data;
				if (root->m_IsWord) std::cout << '\n';
				traverseUtil(os, root->m_Equal);
				traverseUtil(os, root->m_Greater);
			}
		}

		friend std::ostream &operator<<(std::ostream &os, const acTernaryTree &tree)
		{
			tree.traverseUtil(os, tree.m_Root);
			return os;
		}

		void optionUtil(acNode *root, std::vector<std::string> &ac_options, std::string buffer)
		{
			if (!root) return;

			if (root->m_Less) optionUtil(root->m_Less, ac_options, buffer);

			if (root->m_IsWord)
			{
				buffer.append(1, root->m_Data);
				ac_options.push_back(buffer);
			}

			if (root->m_Equal)
			{
				buffer.append(1, root->m_Data);
				optionUtil(root->m_Equal, ac_options, buffer);
			}

			if (root->m_Greater)
			{
				buffer.pop_back();
				optionUtil(root->m_Greater, ac_options, buffer);
			}
		}

		void options(const char *prefix, std::vector<std::string> &ac_options)
		{
			acNode *ptr = m_Root;

			// Check if prefix exists.
			while (ptr)
			{
				if (*prefix < ptr->m_Data)
				{
					ptr = ptr->m_Less;
				}
				else if (*prefix == ptr->m_Data)
				{
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

			optionUtil(ptr, ac_options, "");
		}

	private:
		acNode *m_Root = nullptr;
	};
}

#endif //CCLI_CCLI_AUTOCOMPLETE_H
