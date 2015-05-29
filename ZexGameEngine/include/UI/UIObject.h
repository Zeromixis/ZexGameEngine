#ifndef _UI_UIOBJECT_H_
#define _UI_UIOBJECT_H_

#include "External/boost/uuid/uuid.hpp"
#include "External/boost/uuid/random_generator.hpp"


namespace ZGE
{
	class UIObject
	{
	public:
		UIObject ()
			: m_Uuid ( boost::uuids::random_generator () () )
		{

		}

		~UIObject ()
		{

		}

		const boost::uuids::uuid & Uuid ()
		{
			return m_Uuid;
		}



	private:
		boost::uuids::uuid m_Uuid;
	};
}


#endif // _UI_UIOBJECT_H_