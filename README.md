# CSYS
<table>
    <tr>
        <td>
            master branch
        </td>
        <td>
            Linux/OSX <a href="https://travis-ci.com/rmxbalanque/csys"><img src="https://travis-ci.com/rmxbalanque/csys.svg?branch=master"></a>
        </td>
        <td> 
            Windows <a href="https://ci.appveyor.com/project/rmxbalanque/csys"><img src="https://ci.appveyor.com/api/projects/status/p5e3c6rdysatd6v9/branch/master?svg=true"></a>
        </td>
    </tr>
    <tr>
        <td>
            dev branch
        </td>
        <td>
            Linux/OSX <a href="https://travis-ci.com/rmxbalanque/csys"><img src="https://travis-ci.com/rmxbalanque/csys.svg?branch=development"></a>
        </td>
        <td>
            Windows <a href="https://ci.appveyor.com/project/rmxbalanque/csys"><img src="https://ci.appveyor.com/api/projects/status/p5e3c6rdysatd6v9/branch/development?svg=true"></a>
        </td>
    </tr>
</table>

[![MIT license](https://img.shields.io/badge/License-MIT-green.svg)](https://lbesson.mit-license.org/)
[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/)
[![Standard](https://img.shields.io/badge/c%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)

Modern C++, header-only/compiled, custom console system library.

## Features
- Full and partial tab autocomplete with suggestions.
- Register variables to console system for easy modification.
- Command history support.
- Simple command scripts support.
- Custom data types console logging support. (overload << operator)
- Custom functions with arbitrary arguments registration support. 

## Platforms
- The following platforms are supported:
    - __Windows__: Tested on Windows 10 Professional 64-bit. (Static/Header)
    - __Linux__: Tested on Ubuntu 18.04.4 LTS (Static/Shared/Header)
    
## Install
- __Requirements:__
    - C++17 capable compiler:
        - Visual C++ 15 (Visual Studio 2017)
        - Clang 5
        - GCC 7
    - CMake 3.10+
  
- __Setup:__
    - Header Only
        - Copy source folder and build with C++17 capable compiler. _(CMake not required)_
        
    - Static/Shared Library
        ```console
        $ git clone https://github.com/rmxbalanque/csys.git
        $ cd csys && mkdir build && cd build
        $ cmake .. && make -j
        ```   