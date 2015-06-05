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

		virtual NodeBase *& Parent ()
		{
			return m_Parent;
		}

		virtual ElementType & Position ()
		{
			return m_Position;
		}

		virtual ElementType & Rotation ()
		{
			return m_Rotation;
		}

		virtual ElementType & Scale ()
		{
			return m_Scale;
		}

	private:
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