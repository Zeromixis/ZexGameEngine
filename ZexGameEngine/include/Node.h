#ifndef _NODE_H_
#define _NODE_H_

#include <set>
#include "Math/Vector.h"

namespace ZGE
{
	template < typename Element_Type >
    class NodeBase
    {
    public:
		typedef Element_Type ElementType;

		NodeBase ()
			: m_Parent ( nullptr )
		{

		}

		virtual ~NodeBase ()
		{
			for ( auto &child : m_Childs )
			{
				delete[] child;
			}
		}

		NodeBase *& Parent ()
		{
			return m_Parent;
		}

		const NodeBase *& Parent () const
		{
			return m_Parent;
		}

		ElementType & Position ()
		{
			return m_Position;
		}

		const ElementType & Position () const
		{
			return m_Position;
		}

		ElementType & Rotation ()
		{
			return m_Rotation;
		}

		const ElementType & Rotation () const
		{
			return m_Rotation;
		}

		ElementType & Scale ()
		{
			return m_Scale;
		}

		const ElementType & Scale () const
		{
			return m_Scale;
		}

	protected:
		NodeBase*				m_Parent;
		ElementType				m_Position;
		ElementType				m_Rotation;
		ElementType				m_Scale;
		std::set< NodeBase * >  m_Childs;
    };

    typedef NodeBase< Vector2f > Node2D;
    typedef NodeBase< Vector3f > Node3D;
}
#endif // _NODE_H_