//
// Created by antimatter on 5/26/20.
//

#ifndef CCLI_ARGUMENTS_H
#include "ccli_pch.h"
#include "ccli.h"

namespace ccli
{
  template<typename T>
  struct CCLI_API Arg
  {
    using ValueType = T;
    const std::string& m_Name;
    Arg(const std::string& name) : m_Name(name) {}
  };
}
#define CCLI_ARGUMENTS_H

#endif //CCLI_CCLI_ARGUMENTS_H
