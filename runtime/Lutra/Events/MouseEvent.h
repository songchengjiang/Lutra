//
//  MouseEvent.h
//  Lutra
//
//  Created by JasonCheng on 2021/1/8.
//

#ifndef MouseEvent_h
#define MouseEvent_h
#include "Event.h"
#include "MouseCode.h"

namespace Lutra {

    class MouseEvent : public Event
    {
    public:
        
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
        
    };

    class MouseMovedEvent : public MouseEvent
    {
    public:
        MouseMovedEvent(const float x, const float y)
            : m_mouseX(x), m_mouseY(y) {}

        float GetX() const { return m_mouseX; }
        float GetY() const { return m_mouseY; }

        EVENT_CLASS_TYPE(MouseMoved)
        
    private:
        float m_mouseX, m_mouseY;
    };

    class MouseScrolledEvent : public MouseEvent
    {
    public:
        MouseScrolledEvent(const float xOffset, const float yOffset)
            : m_xOffset(xOffset), m_yOffset(yOffset) {}

        float GetXOffset() const { return m_xOffset; }
        float GetYOffset() const { return m_yOffset; }

        EVENT_CLASS_TYPE(MouseScrolled)
    private:
        float m_xOffset, m_yOffset;
    };

    class MouseButtonEvent : public MouseEvent
    {
    public:
        
        MouseCode GetMouseButton() const { return m_button; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
        
    protected:
        MouseButtonEvent(const MouseCode button)
            : m_button(button) {}

        MouseCode m_button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(const MouseCode button)
            : MouseButtonEvent(button) {}

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(const MouseCode button)
            : MouseButtonEvent(button) {}

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };

}

#endif /* MouseEvent_h */
