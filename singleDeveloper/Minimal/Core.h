//
// Created by Manqing (Stefanie) Zhang on 2019-04-15.
//

#ifndef WHEREISWALDOVR_CORE_H
#define WHEREISWALDOVR_CORE_H


#include <iostream>
#include <memory>
#include <exception>
#include <algorithm>

#include <Windows.h>


#define __STDC_FORMAT_MACROS 1

#define FAIL( X ) throw std::runtime_error(X)

///////////////////////////////////////////////////////////////////////////////
//
// GLM is a C++ math library meant to mirror the syntax of GLSL
//


#include <GL/glew.h>

#include "ovr.hpp"

#include "ExampleApp.hpp"




#endif //WHEREISWALDOVR_CORE_H
