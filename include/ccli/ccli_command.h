//
// Created by antimatter on 5/26/20.
//

#ifndef CCLI_COMMAND_H
#include "ccli_arguments.h"
namespace ccli
{
  template<typename Fn, typename ...Args>
  void registerCommand(const std::string&name, const std::string&description, Fn function, Args... args)
  {
    static_assert(std::is_invocable_v<Fn, typename Args::ValueType...>, "Arguments not valid");
  }
}
#define CCLI_COMMAND_H
#endif //CCLI_CCLI_COMMAND_H
