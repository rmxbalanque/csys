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

		~acTernaryTree()
		{
			deleteTree(m_Root);
		}

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

		void deleteTree(acNode * node)
		{
			if (!node) return;

			// Delete children.
			deleteTree(node->m_Less);
			deleteTree(node->m_Equal);
			deleteTree(node->m_Greater);

			// Delete self.
			delete node;
		}

		// Result.
		bool search(const char *string)
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

			// Continue looking in left branch.
			if (root->m_Less) optionUtil(root->m_Less, ac_options, buffer);

			// Word was found, push into autocomplete options.
			if (root->m_IsWord)
			{
				ac_options.push_back(buffer.append(1, root->m_Data));
				buffer.pop_back();
			}

			// Continue in middle branch, and push character.
			if (root->m_Equal)
			{
				optionUtil(root->m_Equal, ac_options, buffer.append(1, root->m_Data));
				buffer.pop_back();
			}

			// Continue looking in right branch.
			if (root->m_Greater)
			{
				optionUtil(root->m_Greater, ac_options, buffer);
			}
		}

		void options(const char *prefix, std::vector<std::string> &ac_options)
		{
			acNode *ptr = m_Root;
			std::string c_prefix = prefix;

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
			optionUtil(ptr->m_Equal, ac_options, c_prefix);
		}

	private:
		acNode *m_Root = nullptr;
	};
}

#endif //CCLI_CCLI_AUTOCOMPLETE_H
