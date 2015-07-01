#include "UI/UIRoot.h"
#include "App/Context.h"

namespace ZGE
{
    UIRoot::UIRoot ()
    {
        m_Node.Position () = Vector2f ( 0.0f, 0.0f );
        m_Width = Context::GetInstance ()->GetWindowPtr ()->Width ();
    }

    void UIRoot::OnInputEvent ( const InputAction &inputAction )
    {

    }

}