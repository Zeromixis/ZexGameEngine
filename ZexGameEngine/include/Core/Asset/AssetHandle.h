#ifndef _CORE_ASSETHANDLE_H_
#define _CORE_ASSETHANDLE_H_

#include <memory>
#include <xutility>
#include "External/boost/operators.hpp"
#include "Pattern/Object.h"
#include "AssetLoader.h"

namespace ZGE
{
	template< typename T >
	class AssetHandle
		: boost::equality_comparable< AssetHandle< T > >
	{
	public:
		typedef T AssetType;

		friend class AssetLoader< T >;

	public:
		AssetHandle ()
			: m_AssetObject ( nullptr )
		{
			
		}

		AssetHandle ( T* assetPtr )
			: m_AssetObject ( assetPtr )
		{

		}

		AssetHandle ( const AssetHandle &rhs )
			: m_AssetObject ( rhs.m_AssetObject )
		{
			
		}

		AssetHandle ( AssetHandle &&rhs )
			: m_AssetObject ( std::move ( rhs ) )
		{

		}

		AssetHandle & operator = ( const AssetHandle &rhs )
		{
			m_AssetObject = rhs.m_AssetObject;
			return *this;
		}

		AssetHandle & operator = ( AssetHandle &&rhs )
		{
			m_AssetObject = std::move ( rhs.m_AssetObject );
			return *this;
		}

		bool operator == ( const AssetHandle &rhs ) const
		{
			return m_AssetObject == rhs->m_AssetObject;
		}

		T* operator -> () const
		{
			return m_AssetObject.get ();
		}

		T& operator *() const
		{
			return *m_AssetObject;
		}

		explicit operator bool () const
		{
			return m_AssetObject != nullptr;
		}

		T* Get () const
		{
			return m_AssetObject.get ();
		}

		bool IsUnique () const
		{
			return m_AssetObject.unique ();
		}

	private:
		std::shared_ptr< T > m_AssetObject;

	};
}



#endif
