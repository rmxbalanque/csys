//
// Created by antimatter on 5/26/20.
//

#ifndef CCLI_ARGUMENTS_H
#include "ccli_pch.h"
#include "base.h"
#include "ccli.h"
#include <sstream>

namespace ccli
{
  template<typename T>
  struct CCLI_API Arg
  {
    using ValueType = T;
    explicit Arg(CR_STRING name) : m_Name(name), m_Value() {}
    Arg<T>& SetValue(std::stringstream &ss)
    {
      if (!(ss >> m_Value)) throw "Invalid Argument";
      return *this;
    }
    CR_STRING m_Name;
    ValueType m_Value;
  };

  // TODO: Use own char* not string
  // TODO: Parse commandline for 'strings' and "strings"
  // TODO: Support arrays with {} and [] style
  // TODO: to override, if T = {string (accout for '' or ""), arrays with [] and {}, bool (for true and false (optional caps))}
  // TODO: Give better feedback and errors when they mess up
  // TODO: If given MORE arguments than expected, it drops the rest (should give an error and message)
}
#define CCLI_ARGUMENTS_H

#endif //CCLI_ARGUMENTS_H
