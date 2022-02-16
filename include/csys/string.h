// Copyright (c) 2020-present, Roland Munguia & Tristan Florian Bouchard.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef CSYS_STRING_H
#define CSYS_STRING_H
#pragma once

#include <string>
#include <cctype>
#include "csys/api.h"

namespace csys
{
    /*!
        * \brief
        *      Moves until first non-whitespace char, and continues until the end of the string or a whitespace has is
        *      found
        * \param start
        *      Where to start scanning from. Will be set to pair.second
        * \return
        *      Returns the first element and one passed the end of non-whitespace. In other words [first, second)
        */
    inline std::pair<size_t, size_t> NextPoi(const std::string& str, size_t &start)
    {
        size_t end = str.size();
        std::pair<size_t, size_t> range(end + 1, end);
        size_t pos = start;

        // Go to the first non-whitespace char
        for (; pos < end; ++pos)
            if (!std::isspace(str[pos]))
            {
                range.first = pos;
                break;
            }

        // Go to the first whitespace char
        for (; pos < end; ++pos)
            if (std::isspace(str[pos]))
            {
                range.second = pos;
                break;
            }

        start = range.second;
        return range;
    }

    /*!
        * \brief
        *      Returns a value to be compared with the .first of the pair returned from NextPoi
        * \return
        *      Returns size of string + 1
        */
    [[nodiscard]] inline size_t EndPoi(const std::string& str)
    { return str.size() + 1; }
}

#endif //CSYS_CSYS_STRING_H
