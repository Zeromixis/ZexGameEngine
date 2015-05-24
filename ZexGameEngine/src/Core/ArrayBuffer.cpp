#include "Core/ArrayBuffer.h"
#include <assert.h>


namespace ZGE
{
	ArrayBuffer::ArrayBuffer ( U32 size, ArrayBufferUsage usage )
		: m_BufferId ( 0 )
		, m_Size ( size )
		, m_Usage ( usage )
	{
		glGenBuffers ( 1, &m_BufferId );

		assert ( m_Size > 0 );

		m_DataPtr = new U8[ m_Size ];
	}

	ArrayBuffer::~ArrayBuffer ()
	{
		if ( 0 != m_BufferId )
		{
			glDeleteBuffers ( 1, &m_BufferId );
		}
		delete[] m_DataPtr;
	}

	/*
	std::shared_ptr< void >& ArrayBuffer::DataPointer ()
	{
		return m_DataPtr;
	}
	*/

	ZGE::U32 ArrayBuffer::Size ()
	{
		return m_Size;
	}

	void ArrayBuffer::CopyData ( void *src, U32 size )
	{
		memcpy ( m_DataPtr, src, size );
	}

	void ArrayBuffer::TransferData ()
	{
		glBindBuffer ( TARGET, m_BufferId );
		glBufferData ( TARGET, m_Size, m_DataPtr, m_Usage );
	}

	const U32& ArrayBuffer::BufferId ()
	{
		return m_BufferId;
	}

}