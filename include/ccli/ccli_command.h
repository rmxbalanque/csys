//
// Created by antimatter on 5/26/20.
//

#ifndef CCLI_COMMAND_H
#include "ccli_arguments.h"
#include <functional>
#include <type_traits>

namespace ccli
{
  struct CCLI_API CommandBase
  {
    virtual ~CommandBase() = default;
    virtual void operator()(std::string input) = 0;
  };

  template<typename Fn, typename ...Args>
  class CCLI_API Command : public CommandBase
  {
  public:

    Command(CR_STRING name, CR_STRING description, Fn function, Args... args)
            : m_Name(name), m_Description(description), m_Function(function), m_Arguments{args...} {}

    void operator()(std::string input) override
    {
      // call the function
      Call(std::stringstream(input), m_Arguments, std::make_index_sequence<sizeof... (Args)>{});
    }

  private:
    template<typename Tuple, size_t... Is>
    void Call(std::stringstream &&ss, Tuple tuple, std::index_sequence<Is...>)
    {
      // call function with unpacked tuple
      m_Function((std::get<Is>(tuple).SetValue(ss).m_Value)...);
    }

    CR_STRING m_Name;
    CR_STRING m_Description;
    std::function<void(typename Args::ValueType...)> m_Function;
    std::tuple<Args...> m_Arguments;
  };

  template<typename Fn, typename ...Args>
  CCLI_API CommandBase *registerCommand(CR_STRING name, CR_STRING description, Fn function, Args... args)
  {
    // check if function can be called with the given arguments
    static_assert(std::is_invocable_v<Fn, typename Args::ValueType...>, "Arguments specified do not match that of the function");

    // Add commands to system here
    return new Command<Fn, Args...>(name, description, function, args...);
  }
}
#define CCLI_COMMAND_H
#endif //CCLI_COMMAND_H
