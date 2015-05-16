#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <memory>
#include <mutex>

namespace ZGE
{
    template < typename T, bool IsAuto >
    class Singleton
    {
    public:
        static std::shared_ptr< T > GetInstance ()
        {
            if ( Is_Auto && nullptr == m_Instance )
            {
                Construct ();
            }
            return m_Instance;
        }

    protected:
        enum { Is_Auto = IsAuto };

        static void Construct ()
        {
            if ( nullptr != m_Instance.get () )
            {

            }
            else
            {

                std::mutex mtx;
                mtx.lock ();
                if ( nullptr == m_Instance )
                {
                    m_Instance.reset ( new T () );
                }
                mtx.unlock ();
            }
        }

        static void Destroy ()
        {
            m_Instance.reset();
        }

        Singleton ()
        {
            
        }

        ~Singleton ()
        {
            if ( Is_Auto )
            {
                Destroy ();
            }
        }

        static std::shared_ptr< T > m_Instance;

    private:
        Singleton ( const Singleton& );
        Singleton& operator = ( Singleton& );
    };

    template< typename T, bool isAuto >
    std::shared_ptr< T > Singleton< T, isAuto >::m_Instance;
}

#endif

