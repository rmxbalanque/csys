//
// Created by antimatter on 5/26/20.
//

#ifndef CCLI_COMMAND_H
#define CCLI_COMMAND_H
#pragma once

#include <functional>
#include <type_traits>
#include "ccli_arguments.h"
#include "ccli_exceptions.h"

namespace ccli
{
  struct  CommandBase
  {
    virtual ~CommandBase() = default;
    virtual void operator()(String &input) = 0;
    virtual std::string Help() = 0;
  };

  template<typename Fn, typename ...Args>
  class CCLI_API Command : public CommandBase
  {
  public:
    Command(const String &name, const String &description, Fn function, Args... args)
            : m_Name(name), m_Description(description), m_Function(function), m_Arguments(args...) {}

    void operator()(String &input) override
    {
      // call the function
			unsigned long start = 0;
			try { Call(input, start, std::make_index_sequence<sizeof... (Args)>{}); }
			catch (ArgumentException ae) { throw CommandException("[ERROR] " + m_Name.m_String + ": " + ae.what()); }
    }

    std::string Help() override
    {
			return "COMMAND: " + m_Name.m_String + "\n"
			     + "     Description - " + m_Description.m_String + "\n\n"
					 + "     Usage - " + m_Name.m_String + DisplayArguments(std::make_index_sequence<sizeof... (Args)>{}) + "\n\n";
    }

  private:
    template<size_t... Is>
    void Call(String &input, unsigned long &start, std::index_sequence<Is...>)
    {
      // call function with unpacked tuple
      m_Function((std::get<Is>(m_Arguments).Parse(input, start).m_Arg.m_Value)...);
    }

    template<size_t ...Is>
    std::string DisplayArguments(std::index_sequence<Is...>)
    {
    	return (std::get<Is>(m_Arguments).Info() + ...);
    }

    const String m_Name;
		const String m_Description;
    std::function<void(typename Args::ValueType...)> m_Function;
    std::tuple<Args...> m_Arguments;
  };

	template<typename Fn>
	class CCLI_API Command<Fn> : public CommandBase
	{
	public:
		Command(const String &name, const String &description, Fn function)
						: m_Name(name), m_Description(description), m_Function(function) {}

		void operator()(String &input) override
		{
			// call the function
			for (auto c : input.m_String)
				if (!isspace(c))
					throw CommandException("[ERROR] " + m_Name.m_String + ": Called with arguments");

			try { m_Function(); }
			catch (...) { throw CommandException("[ERROR] " + m_Name.m_String + ": Called function resulted in exception thrown"); }
		}

		std::string Help() override
		{
			return "COMMAND: " + m_Name.m_String + "\n"
			     + "     Description - " + m_Description.m_String + "\n\n"
					 + "     Usage - " + m_Name.m_String + "\n\n";
		}

	private:
		template<size_t... Is>
		void Call(String &input, unsigned long &start, std::index_sequence<Is...>) {}

		template<size_t ...Is>
		std::string DisplayArguments(std::index_sequence<Is...>) { return ""; }

		const String m_Name;
		const String m_Description;
		std::function<void(void)> m_Function;
		std::tuple<> m_Arguments;
	};

	template<typename Fn, typename ...Args>
  CommandBase *registerCommand(const String &name, const String &description, Fn function, Args... args)
  {
    // check if function can be called with the given arguments
    static_assert(std::is_invocable_v<Fn, typename Args::ValueType...>, "Arguments specified do not match that of the function");

    // Add commands to system here
    return new Command<Fn, Args...>(name, description, function, args...);
  }
}

#endif //CCLI_COMMAND_H
