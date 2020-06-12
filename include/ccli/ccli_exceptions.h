//
// Created by antimatter on 6/1/20.
//

#ifndef CCLI_EXCEPTIONS_H
#define CCLI_EXCEPTIONS_H
#pragma once

#include <string>
#include <exception>
#include "ccli_api.h"

namespace ccli
{
	class CCLI_API ArgumentException: public std::exception
	{
	public:
		explicit ArgumentException(const std::string &message, const std::string & arg):
		msg_(message + ": '" + arg + "'")
		{}

		~ArgumentException() override = default;

		[[nodiscard]] const char* what() const noexcept override
		{
			return msg_.c_str();
		}

	protected:
		std::string msg_;
	};

}

#endif //CCLI_CCLI_EXCEPTIONS_H
