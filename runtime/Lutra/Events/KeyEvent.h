//
//  KeyEvent.h
//  Lutra
//
//  Created by JasonCheng on 2021/1/8.
//

#ifndef KeyEvent_h
#define KeyEvent_h
#include "Event.h"
#include "KeyCode.h"

namespace Lutra {

    class KeyEvent : public Event
    {
    public:
        KeyCode GetKeyCode() const { return m_KeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
        
    protected:
        KeyEvent(const KeyCode keycode)
            : m_KeyCode(keycode) {}

        KeyCode m_KeyCode;
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(const KeyCode keycode, const uint16_t repeatCount)
            : KeyEvent(keycode), m_repeatCount(repeatCount) {}

        uint16_t GetRepeatCount() const { return m_repeatCount; }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        uint16_t m_repeatCount;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(const KeyCode keycode)
            : KeyEvent(keycode) {}

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class KeyTypedEvent : public KeyEvent
    {
    public:
        KeyTypedEvent(const KeyCode keycode)
            : KeyEvent(keycode) {}

        EVENT_CLASS_TYPE(KeyTyped)
    };

}

#endif /* KeyEvent_h */
