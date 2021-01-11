//
//  Event.h
//  Lutra
//
//  Created by JasonCheng on 2021/1/8.
//

#ifndef Event_h
#define Event_h

#include <unordered_map>

namespace Lutra {

    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum EventCategory
    {
        None = 0,
        EventCategoryWindow         = 1,
        EventCategoryInput          = 1<<1,
        EventCategoryKeyboard       = 1<<2,
        EventCategoryMouse          = 1<<3,
        EventCategoryMouseButton    = 1<<4
    };

#define EVENT_CLASS_TYPE(type)  static EventType GetStaticType() { return EventType::type; }\
                                virtual EventType GetEventType() const override { return GetStaticType(); }\
                                virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

    class Event
    {
    public:
        virtual ~Event() = default;
        
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;

        bool IsInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }
    };

    class EventDispatcher
    {
    public:
        
        using EventHandleFn = std::function<bool(Event&)>;
        
        bool Dispatch()
        {
            for (auto& event : m_eventList) {
                auto iter = m_bindingFn.find(event->GetEventType());
                if (iter != m_bindingFn.end()) {
                    for (auto& fn : iter->second) {
                        if (fn(*event.get()))
                            break;
                    }
                }
            }
            m_eventList.clear();
            return true;
        }
        
        void Send(const std::shared_ptr<Event>&& event)
        {
            m_eventList.emplace_back(std::move(event));
        }
        
        template<typename T>
        void Connect(const EventHandleFn& fn)
        {
            m_bindingFn[T::GetStaticType()].push_back(fn);
        }
        
    private:
        
        std::vector<std::shared_ptr<Event>> m_eventList;
        std::unordered_map<EventType, std::vector<EventHandleFn>> m_bindingFn;
    };

}


#endif /* Event_h */
