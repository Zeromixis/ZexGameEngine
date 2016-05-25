#pragma once

#include "ZGEDecl.h"
#include "ZGEDef.h"

#include "External/boost/noncopyable.hpp"


namespace ZGE
{
    enum ArrayBufferUsage
    {
        STATIC = GL_STATIC_DRAW,
        DYNAMIC = GL_DYNAMIC_DRAW,
    };

    template< int BindTarget >
	class BufferGL 
        : private boost::noncopyable
	{
	public:
        enum
        {
            TARGET = BindTarget,
        };

        BufferGL ()
            : m_BufferId ( 0 )
            , m_Size ( 0 )
            , m_Usage ( STATIC )
        {

        }

        BufferGL ( U32 byteSize, ArrayBufferUsage usage )
            : m_BufferId ( 0 )
            , m_Size ( byteSize )
            , m_Usage ( usage )
        {
            glGenBuffers ( 1, &m_BufferId );

            assert ( m_Size > 0 );

            m_DataPtr = new U8[ m_Size ];
        }

        ~BufferGL ()
        {
            if ( 0 != m_BufferId )
            {
                glDeleteBuffers ( 1, &m_BufferId );
            }
            delete[] m_DataPtr;
        }

        const U32& Handle ()
        {
            return m_BufferId;
        }

        void CopyData ( void *src, U32 byteSize )
        {
            memcpy ( m_DataPtr, src, byteSize );
        }

        void SendData ()
        {
            glBindBuffer ( TARGET, m_BufferId );
            glBufferData ( TARGET, m_Size, m_DataPtr, m_Usage );
        }

        U32 Size ()
        {
            return m_Size;
        }

	private:
		U32 m_BufferId;
		U32 m_Size;
		ArrayBufferUsage m_Usage;
		U8 *m_DataPtr;
	};
}