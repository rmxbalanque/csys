// Copyright (c) 2020-present, Roland Munguia & Tristan Florian Bouchard.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef CSYS_EXCEPTIONS_H
#define CSYS_EXCEPTIONS_H
#pragma once

#include <string>
#include <exception>
#include <utility>
#include "csys/api.h"

namespace csys
{
    /*!
     * \brief
     *
     */
    class CSYS_API Exception : public std::exception
    {
    public:
        /*!
         * \brief
         *
         * \param message
         *
         * \param arg
         *
         */
        explicit Exception(const std::string &message, const std::string &arg) : m_Msg(message + ": '" + arg + "'")
        {}

        /*!
         * \brief
         *
         * \param message
         *
         */
        explicit Exception(std::string message) : m_Msg(std::move(message))
        {}

        /*!
         * \brief
         *
         */
        ~Exception() override = default;

        /*!
         * \brief
         *
         * \return
         *
         */
        [[nodiscard]] const char *what() const noexcept override
        {
            return m_Msg.c_str();
        }

    protected:
        std::string m_Msg;    //!<
    };
}

#endif //CSYS_CSYS_EXCEPTIONS_H
