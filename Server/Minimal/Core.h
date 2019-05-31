//
// Created by Jonathan Leung
//

#ifndef CAVE_CORE_H
#define CAVE_CORE_H


#include <iostream>
#include <memory>
#include <exception>
#include <algorithm>

#include <Windows.h>

#define __STDC_FORMAT_MACROS 1

#define FAIL(X) throw std::runtime_error(X)

///////////////////////////////////////////////////////////////////////////////
//
// GLM is a C++ math library meant to mirror the syntax of GLSL
//


#include <GL/glew.h>

#include "ovr.hpp" 

#include "CaveApp.hpp"

#endif //CAVE_CORE_H
