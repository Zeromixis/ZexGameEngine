#ifndef _PATTERN_SINGLETON_H_
#define _PATTERN_SINGLETON_H_

/******************************************************************************************************************************************/
/* Thanks to Technical reference which coming from																						  */
/* http://www.klayge.org/2015/04/27/c%E4%B8%AD%E7%BA%BF%E7%A8%8B%E5%AE%89%E5%85%A8%E5%B9%B6%E4%B8%94%E9%AB%98%E6%95%88%E7%9A%84singleton/ */
/******************************************************************************************************************************************/

#include <mutex>
#include <atomic>
#include "External/boost/noncopyable.hpp"

namespace ZGE
{
    template < typename T >
    class Singleton : boost::noncopyable
    {
    public:
        static T* GetInstance ()
        {
			T* tmp = m_Instance.load ( std::memory_order_consume );
			if ( nullptr == tmp )
			{
				std::unique_lock< std::mutex > lock ( m );
				tmp = m_Instance.load ( std::memory_order_relaxed );
				if ( nullptr == tmp )
				{
					m_Instance.store ( new T (), std::memory_order_release );
				}
			}
            return m_Instance;
        }

    protected:
        static void Destroy ()
        {
			T* tmp = m_Instance.load ();
			if ( nullptr != tmp )
			{
				delete tmp;
				m_Instance.store ( nullptr );
			}
        }

        Singleton ()
        {
            
        }

        ~Singleton ()
        {
			Destroy ();
        }

	private:
        static std::atomic< T* > m_Instance;
		static std::mutex m;
    };

	template< typename T >
	std::atomic< T* > Singleton< T >::m_Instance;

	template< typename T >
	std::mutex Singleton< T >::m;
}

#endif // _PATTERN_SINGLETON_H_

