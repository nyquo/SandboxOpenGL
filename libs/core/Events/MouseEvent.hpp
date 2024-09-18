#pragma once

#include "CoreExport.hpp"
#include "Event.hpp"

#ifndef NDEBUG
#include <sstream>
#endif

namespace core {

class MouseScrolledEvent : public Event
{
  public:
    MouseScrolledEvent(float xOffset, float yOffset)
      : m_xOffset(xOffset)
      , m_yOffset(yOffset)
    {}

    float getXOffset() const { return m_xOffset; }
    float getYOffset() const { return m_yOffset; }

    EVENT_CLASS_CATEGORY(EventCategoryMouseEvent)
    EVENT_CLASS_TYPE(MouseScrolledEvent)

#ifndef NDEBUG
    std::string toString() const override
    {
        std::stringstream ss;
        ss << getEventName() << " xOffset: " << m_xOffset << " | yOffset: " << m_yOffset;
        return ss.str();
    }
#endif

  private:
    float m_xOffset, m_yOffset;
};

}
