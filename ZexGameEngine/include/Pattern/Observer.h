#pragma once

#include "ZGEDecl.h"

namespace ZGE
{
	template < typename... Args >
	class Observer;

	template < typename... Args >
	class Subject
	{
	public:
		void AddObserver ( Observer< Args... > &ob )
		{
			m_ObserverList.insert ( &ob );
		}

		void RemoveObserver ( Observer< Args... > &ob )
		{
			m_ObserverList.erase ( &ob );
		}

		void NotifyAll ( Args... args )
		{
			for ( auto &ob : m_ObserverList )
			{
				ob->OnNotify ( *this, args... );
			}
		}

	private:
		std::set < Observer< Args... > * > m_ObserverList;
	};

	template < typename... Args >
	class Observer
	{
	public:
		virtual void OnNotify ( Subject< typename Args... > &subject, Args... args) = 0;
	};
}
