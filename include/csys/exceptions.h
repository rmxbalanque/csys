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
	class CSYS_API Exception : public std::exception
	{
	public:
		explicit Exception(const std::string &message, const std::string &arg) :
						m_Msg(message + ": '" + arg + "'")
		{ /*std::cout << "EXCEPTION: " << what() << std::endl;*/ }

		explicit Exception(std::string message) :
						m_Msg(std::move(message))
		{}

		~Exception() override = default;

		[[nodiscard]] const char *what() const noexcept override
		{
			return m_Msg.c_str();
		}

	protected:
		std::string m_Msg;
	};
}

#endif //CSYS_CSYS_EXCEPTIONS_H
