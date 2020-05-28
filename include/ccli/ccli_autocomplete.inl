
#pragma once

#ifndef CCLI_HEADER_ONLY
#include "ccli_autocomplete.h"
#endif

namespace ccli
{
	///////////////////////////////////////////////////////////////////////////
	// Constructor/Destructors ////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	CCLI_INLINE acTernarySearchTree::~acTernarySearchTree()
	{
		delete m_Root;
	}

	///////////////////////////////////////////////////////////////////////////
	// Public methods /////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	CCLI_INLINE bool acTernarySearchTree::search(const char *string)
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

	CCLI_INLINE void acTernarySearchTree::insert(const char *str)
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

	CCLI_INLINE void acTernarySearchTree::insert(const std::string &str)
	{
		insert(str.data());
	}

	CCLI_INLINE void acTernarySearchTree::suggestions(const char *prefix, std::vector<std::string> &ac_options)
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

	CCLI_INLINE acTernarySearchTree::p_sVector acTernarySearchTree::suggestions(const char *prefix)
	{
		auto temp = new sVector();
		suggestions(prefix, *temp);
		return temp;
	}

	///////////////////////////////////////////////////////////////////////////
	// Private methods ////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	CCLI_INLINE void acTernarySearchTree::suggestionsAux(acTernarySearchTree::acNode *root, r_sVector ac_options, std::string buffer)
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
}