#pragma once

#include "ZGEDecl.h"

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

        const NodeBase * const & Parent () const
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

		const ElementType GlobalPosition () const
		{
			ElementType result = Position ();
			NodeBase *parent = Parent ();
			while ( nullptr != parent )
			{
				result += parent->Position ();
				parent = parent->Parent ();
			}
			return result;
		}

		ElementType & Rotation ()
		{
			return m_Rotation;
		}

		const ElementType & Rotation () const
		{
			return m_Rotation;
		}

		const ElementType GlobalRotation () const
		{
			ElementType result = Rotation ();
			NodeBase *parent = Parent ();
			while ( nullptr != parent )
			{
				result += parent->Rotation ();
				parent = parent->Parent ();
			}
			return result;
		}

		ElementType & Scale ()
		{
			return m_Scale;
		}

		const ElementType & Scale () const
		{
			return m_Scale;
		}

		const ElementType GlobalScale () const
		{
			ElementType result = Scale ();
			NodeBase *parent = Parent ();
			while ( nullptr != parent )
			{
				result *= parent->Scale ();
				parent = parent->Parent ();
			}
			return result;
		}

        std::set< NodeBase * > & Childs ()
        {
            return m_Childs;
        }

        const std::set< NodeBase * > & Childs () const
        {
            return m_Childs;
        }

	protected:
		NodeBase*				m_Parent;
		ElementType				m_Position;
		ElementType				m_Rotation;
		ElementType				m_Scale;
		std::set< NodeBase * >  m_Childs;

		// Maybe be used in the future.
		//bool					m_Dirty;
    };

    typedef NodeBase< Vector2f > Node2D;
    typedef NodeBase< Vector3f > Node3D;
}

