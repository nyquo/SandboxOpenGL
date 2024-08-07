#pragma once
#include "Event.hpp"

#ifndef NDEBUG
#include <sstream>
#endif

namespace core {

    class WindowCloseEvent : public Event
    {
    public:
        EVENT_CLASS_CATEGORY(EventCategoryWindowEvent)
            EVENT_CLASS_TYPE(WindowCloseEvent)

#ifndef NDEBUG
            std::string toString() const override
        {
            std::stringstream ss;
            ss << getEventName() << "Window Close";
            return ss.str();
        }
#endif
    };

    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height)
            : m_width(width)
            , m_height(height)
        {}

        unsigned int getWidth() const { return m_width; }
        unsigned int getHeight() const { return m_height; }

        EVENT_CLASS_CATEGORY(EventCategoryWindowEvent)
            EVENT_CLASS_TYPE(WindowResizeEvent)

#ifndef NDEBUG
            std::string toString() const override
        {
            std::stringstream ss;
            ss << getEventName() << "Window Close";
            return ss.str();
        }
#endif

    private:
        unsigned int m_width, m_height;
    };

}