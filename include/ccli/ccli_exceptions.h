//
// Created by antimatter on 6/1/20.
//

#ifndef CCLI_EXCEPTIONS_H
#define CCLI_EXCEPTIONS_H
#pragma once

#include <string>
#include <exception>
#include <utility>
#include "ccli/ccli_api.h"

namespace ccli
{
	class CCLI_API Exception : public std::exception
	{
	public:
		explicit Exception(const std::string &message, const std::string &arg) :
				m_Msg(message + ": '" + arg + "'")
		{}

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

#endif //CCLI_CCLI_EXCEPTIONS_H
