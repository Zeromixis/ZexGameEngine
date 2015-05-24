#ifndef _CORE_VERTEXBUFFER_H_
#define _CORE_VERTEXBUFFER_H_

#include <memory>
#include "DataDef.h"
#include "External/boost/noncopyable.hpp"


namespace ZGE
{
	class ArrayBuffer : boost::noncopyable
	{
	public:
		enum ArrayBufferUsage
		{
			Static  = GL_STATIC_DRAW,
			Dynamic = GL_DYNAMIC_DRAW,
		};

		//VertexBuffer ();

		ArrayBuffer ( U32 size, ArrayBufferUsage usage );

		~ArrayBuffer ();

		//std::shared_ptr< void >& DataPointer ();

		const U32& BufferId ();

		void CopyData ( void *src, U32 size );

		void TransferData ();

		U32 Size ();

	protected:
		enum
		{ 
			TARGET = GL_ARRAY_BUFFER, 
		};

	private:
		U32 m_BufferId;
		U32 m_Size;
		ArrayBufferUsage m_Usage;
		U8 *m_DataPtr;
	};
}

#endif