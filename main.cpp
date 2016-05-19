#include "ZGEDecl.h"

#include "App/Game.h"
#include "ZexGameEngine/include/Math/Vector.h"

class B;

template< typename T, std::size_t N >
class A
{
public:
    A (int i)
    {
        m_Array.fill (i);
    }

    A (const A &&rhs)
    {

    }

    constexpr A (const B &)
    {

    }

    template< typename U >
    A (const U &)
    {

    }

    constexpr A (const std::array< T, N > &array)
        : m_Array (array)
    {

    }

    T & operator [] (const std::size_t &index)
    {
        return m_Array [index];
    }

    constexpr const T & operator [] (const std::size_t &index) const
    {
        return m_Array [index];
    }

private:
    std::array< T, N > m_Array;
};


int WINAPI WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdWindow )
{
    using namespace ZGE;
    Vector4f ss{1.0f, 1.0f};

    ZGE::Game ZexGame;
    ZexGame.Init ();
    ZexGame.Loop ();
    ZexGame.Finalize ();
}

