//
// Created by antimatter on 5/26/20.
//

#ifndef CCLI_COMMAND_H
#include "ccli_arguments.h"
#include <functional>
#include <type_traits>

namespace ccli
{
  struct  CommandBase
  {
    virtual ~CommandBase() = default;
    virtual void operator()(string &input) = 0;
    virtual std::string Info() = 0;
  };

  template<typename Fn, typename ...Args>
  class CCLI_API Command : public CommandBase
  {
  public:
    Command(const string &name, const string &description, Fn function, Args... args)
            : m_Name(name), m_Description(description), m_Function(function), m_Arguments{args...} {}

    void operator()(string &input) override
    {
      // call the function
      unsigned long start = 0;
      Call(input, start, std::make_index_sequence<sizeof... (Args)>{});
    }

    std::string Info() override
    {
    	std::string info = "NAME: " + m_Name.m_String + "\n";
    	           info += "DESCRIPTION: " + m_Description.m_String + "\n";
    	           info += "USAGE: " + m_Name.m_String + " " + GetArgInfo(std::make_index_sequence<sizeof... (Args)>{});
    	return info;
    }

  private:
    template<size_t... Is>
    void Call(string &input, unsigned long &start, std::index_sequence<Is...>)
    {
      // call function with unpacked tuple
      m_Function((std::get<Is>(m_Arguments).Parse(input, start).m_Arg.m_Value)...);
    }

    template<size_t ...Is>
    std::string GetArgInfo(std::index_sequence<Is...>)
    {
    	return (std::get<Is>(m_Arguments).Info() + ...) + "\n";
    }

    const string &m_Name;
		const string &m_Description;
    std::function<void(typename Args::ValueType...)> m_Function;
    std::tuple<Args...> m_Arguments;
  };

	template<typename Fn, typename ...Args>
  CommandBase *registerCommand(const string &name, const string &description, Fn function, Args... args)
  {
    // check if function can be called with the given arguments
    static_assert(std::is_invocable_v<Fn, typename Args::ValueType...>, "Arguments specified do not match that of the function");

    // Add commands to system here
    return new Command<Fn, Args...>(name, description, function, args...);
  }
}
#define CCLI_COMMAND_H
#endif //CCLI_COMMAND_H
