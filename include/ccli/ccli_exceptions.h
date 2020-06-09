//
// Created by antimatter on 6/1/20.
//

#ifndef CCLI_EXCEPTIONS_H
#define CCLI_EXCEPTIONS_H
#pragma once

#include <string>
#include "ccli_api.h"

namespace ccli
{
	struct CCLI_API Exception
	{
		virtual std::string what() const = 0;
	};

	class CCLI_API CommandException : public Exception
	{
	public:
		CommandException(const std::string& msg) : m_What(msg) {}
		std::string what() const override { return m_What; }
	private:
		std::string m_What;
	};

	class CCLI_API ArgumentException : public Exception
	{
	public:
		ArgumentException(const std::string& msg) : m_What(msg) {}
		std::string what() const override { return m_What; };
	private:
		std::string m_What;
	};
}

#endif //CCLI_CCLI_EXCEPTIONS_H
