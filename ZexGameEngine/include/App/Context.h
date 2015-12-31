#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include "ZGEDecl.h"

#include "Pattern/Singleton.h"
#include "Window.h"
#include "Render/RenderEngine.h"

namespace ZGE
{
	class RenderEngine;

    class Context : public Singleton< Context >
    {
        friend class Singleton< Context >;
    public:
        ~Context () {};

        std::shared_ptr< Window > GetWindowPtr ()
        {
            return m_Window;
        }

        void SetWindowPtr ( Window *window )
        {
            m_Window.reset ( window );
        }
        
        std::shared_ptr< RenderEngine > GetRenderEngine ()
        {
            return m_RenderEngine;
        }

        void SetRenderEngine ( RenderEngine *renderEngine )
        {
            m_RenderEngine.reset ( renderEngine );
        }

    private:
        Context ()
        {
           
        }

        std::shared_ptr< Window >       m_Window;
        std::shared_ptr< RenderEngine > m_RenderEngine;

    };
}

#endif //_CONTEXT_H_