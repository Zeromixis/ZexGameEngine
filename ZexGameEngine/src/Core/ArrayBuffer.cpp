// #include "Core/ArrayBuffer.h"
// #include <assert.h>
// 
// 
// namespace ZGE
// {
//     template< int BindTarget >
// 	ArrayBuffer< BindTarget >::ArrayBuffer ( U32 byteSize, ArrayBufferUsage usage )
// 		: m_BufferId ( 0 )
// 		, m_Size ( byteSize )
// 		, m_Usage ( usage )
// 	{
// 		glGenBuffers ( 1, &m_BufferId );
// 
// 		assert ( m_Size > 0 );
// 
// 		m_DataPtr = new U8[ m_Size ];
// 	}
// 
//     template< int BindTarget >
// 	ArrayBuffer< BindTarget >::~ArrayBuffer ()
// 	{
// 		if ( 0 != m_BufferId )
// 		{
// 			glDeleteBuffers ( 1, &m_BufferId );
// 		}
// 		delete[] m_DataPtr;
// 	}
// 
//     template< int BindTarget >
// 	ZGE::U32 ArrayBuffer< BindTarget >::Size ()
// 	{
// 		return m_Size;
// 	}
// 
//     template< int BindTarget >
// 	void ArrayBuffer< BindTarget >::CopyData ( void *src, U32 size )
// 	{
// 		memcpy ( m_DataPtr, src, size );
// 	}
// 
//     template< int BindTarget >
// 	void ArrayBuffer< BindTarget >::SendData ()
// 	{
// 		glBindBuffer ( TARGET, m_BufferId );
// 		glBufferData ( TARGET, m_Size, m_DataPtr, m_Usage );
// 	}
// 
//     template< int BindTarget >
// 	const U32& ArrayBuffer< BindTarget >::Handle ()
// 	{
// 		return m_BufferId;
// 	}
// }