#ifndef _ZGEDECL_H_
#define _ZGEDECL_H_

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
#include <assert.h>

#include "glloader/glloader.h"
#include "ZGEDef.h"
#include "Object.h"
#include "Util.h"

namespace ZGE
{
    class Object;
    class ShaderCode;
    class ShaderProgram;
    
    template< typename T, size_t N >
    class Vector;
    class VectorHelper;

    template < typename T, size_t row, size_t col > 
    class Matrix;
    template < typename T >
    class Matrix44;
}

#endif // !_ZGE_DECL_H_


