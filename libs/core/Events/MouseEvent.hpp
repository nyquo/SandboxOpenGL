#pragma once

#include "CoreExport.hpp"
#include "Event.hpp"

#ifndef NDEBUG
#include <sstream>
#endif

namespace core {

class CORE_API MouseMovedEvent : public Event
{
  public:
    MouseMovedEvent(float x, float y)
      : m_posX(x)
      , m_posY(y)
    {}

    float getX() const { return m_posX; }
    float getY() const { return m_posY; }

    EVENT_CLASS_CATEGORY(EventCategoryMouseEvent)
    EVENT_CLASS_TYPE(MouseMovedEvent)

#ifndef NDEBUG
    std::string toString() const override
    {
        std::stringstream ss;
        ss << getEventName() << " x: " << m_posX << " | y: " << m_posY;
        return ss.str();
    }
#endif

  private:
    float m_posX, m_posY;
};

class CORE_API MouseScrolledEvent : public Event
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
