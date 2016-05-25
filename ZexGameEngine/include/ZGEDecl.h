#pragma once

/*
    The Global Forward Declaration Of ZexGameEngine.
*/

#include <iostream>
#include <memory>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <list>
#include <deque>
#include <stack>
#include <functional>
#include <array>
#include <sstream>
#include <xutility>
#include <cmath>
#include <map>
#include <type_traits>
#include <assert.h>

#include "glloader/glloader.h"

namespace ZGE
{
    struct Property;
    class Object;
    class ShaderCode;
    class ShaderProgram;
    
    template< typename T, size_t N >
    class Vector;

    class VectorHelper;

    template< typename T >
    class Matrix44;

    template< typename T >
    class Matrix44ColProxy;

    class Quaternion;
}


