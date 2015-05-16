#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <memory>
#include "Singleton.h"
#include "Window.h"
#include "Render/RenderEngine.h"

namespace ZGE
{
    class Context : public Singleton< Context, true >
    {
        friend class Singleton< Context, true >;
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