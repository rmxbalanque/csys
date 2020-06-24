// Copyright (c) 2020-present, Roland Munguia & Tristan Florian Bouchard.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef CSYS_COMMAND_H
#define CSYS_COMMAND_H
#pragma once

#include <functional>
#include <type_traits>
#include <utility>
#include "csys/arguments.h"
#include "csys/exceptions.h"
#include "csys/item.h"

namespace csys
{
    /*!
     * \brief
     *
     */
    struct CommandBase
    {
        /*!
         * \brief
         *
         */
        virtual ~CommandBase() = default;

        /*!
         * \brief
         *
         * \param input
         *
         * \return
         *
         */
        virtual Item operator()(String &input) = 0;

        /*!
         * \brief
         *
         * \return
         *
         */
        [[nodiscard]] virtual std::string Help() = 0;

        /*!
         * \brief
         *
         * \return
         *
         */
        [[nodiscard]] virtual size_t ArgumentCount() const = 0;
    };

    /*!
     * \brief
     *
     * \tparam Fn
     *
     * \tparam Args
     *
     */
    template<typename Fn, typename ...Args>
    class CSYS_API Command : public CommandBase
    {
    public:
        /*!
         * \brief
         *
         * \param name
         *
         * \param description
         *
         * \param function
         *
         * \param args
         *
         */
        Command(String name, String description, Fn function, Args... args) : m_Name(std::move(name)),
                                                                              m_Description(std::move(description)),
                                                                              m_Function(function),
                                                                              m_Arguments(args..., Arg<NULL_ARGUMENT>())
        {}

        /*!
         * \brief
         *
         * \param input
         *
         * \return
         *
         */
        Item operator()(String &input) override
        {
            // try to parse and call the function
            try
            {
                Call(input, std::make_index_sequence<sizeof ...(Args) + 1>{},
                     std::make_index_sequence<(sizeof ...(Args))>{});
            }
            catch (Exception &ae)
            { return Item(ERROR) << (m_Name.m_String + ": " + ae.what()); }
            return Item(NONE);
        }

        /*!
         * \brief
         *
         * \return
         *
         */
        [[nodiscard]] std::string Help() override
        {
            return m_Name.m_String + DisplayArguments(std::make_index_sequence<sizeof ...(Args)>{}) + "\n\t\t- " +
                   m_Description.m_String + "\n\n";
        }

        /*!
         * \brief
         *
         * \return
         *
         */
        [[nodiscard]] size_t ArgumentCount() const override
        {
            return sizeof... (Args);
        }

    private:
        /*!
         * \brief
         *
         * \tparam Is_p
         *
         * \tparam Is_c
         *
         * \param input
         *
         */
        template<size_t... Is_p, size_t... Is_c>
        void Call(String &input, const std::index_sequence<Is_p...> &, const std::index_sequence<Is_c...> &)
        {
            size_t start = 0;

            // parse arguments
            int _[]{0, (void(std::get<Is_p>(m_Arguments).Parse(input, start)), 0)...};
            (void) (_);

            // call function with unpacked tuple
            m_Function((std::get<Is_c>(m_Arguments).m_Arg.m_Value)...);
        }

        /*!
         * \brief
         *
         * \tparam Is
         *
         * \return
         *
         */
        template<size_t ...Is>
        std::string DisplayArguments(const std::index_sequence<Is...> &)
        {
            return (std::get<Is>(m_Arguments).Info() + ...);
        }

        const String m_Name;                                            //!<
        const String m_Description;                                     //!<
        std::function<void(typename Args::ValueType...)> m_Function;    //!<
        std::tuple<Args..., Arg<NULL_ARGUMENT>> m_Arguments;            //!<
    };

    /*!
     * \brief
     *
     * \tparam Fn
     *
     */
    template<typename Fn>
    class CSYS_API Command<Fn> : public CommandBase
    {
    public:
        /*!
         * \brief
         *
         * \param name
         *
         * \param description
         *
         * \param function
         *
         */
        Command(String name, String description, Fn function) : m_Name(std::move(name)),
                                                                m_Description(std::move(description)),
                                                                m_Function(function), m_Arguments(Arg<NULL_ARGUMENT>())
        {}

        /*!
         * \brief
         *
         * \param input
         *
         * \return
         *
         */
        Item operator()(String &input) override
        {
            // call the function
            size_t start = 0;
            try
            { std::get<0>(m_Arguments).Parse(input, start); }
            catch (Exception &ae)
            {
                return Item(ERROR) << (m_Name.m_String + ": " + ae.what());
            }
            m_Function();
            return Item(NONE);
        }

        /*!
         * \brief
         *
         * \return
         *
         */
        [[nodiscard]] std::string Help() override
        {
            return m_Name.m_String + "\n\t\t- " + m_Description.m_String + "\n\n";
        }

        /*!
         * \brief
         *
         * \return
         *
         */
        [[nodiscard]] size_t ArgumentCount() const override
        {
            return 0;
        }

    private:

        const String m_Name;                           //!<
        const String m_Description;                    //!<
        std::function<void(void)> m_Function;          //!<
        std::tuple<Arg<NULL_ARGUMENT>> m_Arguments;    //!<
    };
}

#endif //CSYS_COMMAND_H
