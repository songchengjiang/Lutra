//
//  KeyEvent.h
//  Lutra
//
//  Created by JasonCheng on 2021/1/8.
//

#ifndef KeyEvent_h
#define KeyEvent_h
#include "Event.h"

namespace Lutra {

    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height)
            : m_width(width), m_height(height) {}

        unsigned int GetWidth() const { return m_width; }
        unsigned int GetHeight() const { return m_height; }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryWindow)
    private:
        unsigned int m_width, m_height;
    };

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;
        
        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryWindow)
    };

}

#endif /* KeyEvent_h */
