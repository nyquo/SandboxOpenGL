#pragma once

#include "Event.hpp"
#include "core/CoreExport.hpp"

#include <filesystem>
#include <vector>

#ifndef NDEBUG
#include <sstream>
#endif

namespace core {

class CORE_API WindowCloseEvent : public Event
{
  public:
    EVENT_CLASS_CATEGORY(EventCategoryWindowEvent)
    EVENT_CLASS_TYPE(WindowCloseEvent)

#ifndef NDEBUG
    std::string toString() const override
    {
        std::stringstream ss;
        ss << getEventName();
        return ss.str();
    }
#endif
};

class CORE_API WindowResizeEvent : public Event
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
        ss << getEventName() << " width: " << m_width << " | height: " << m_height;
        return ss.str();
    }
#endif

  private:
    unsigned int m_width, m_height;
};

class CORE_API FileDropEvent : public Event
{
  public:
    FileDropEvent(int count, const char** paths)
    {
        m_filePaths.reserve(count);
        for(int i = 0; i < count; ++i)
        {
            m_filePaths.emplace_back(paths[i]);
        }
    }

    const std::vector<std::filesystem::path>& getPaths() const { return m_filePaths; }

    EVENT_CLASS_CATEGORY(EventCategoryWindowEvent)
    EVENT_CLASS_TYPE(FileDropEvent)

#ifndef NDEBUG
    std::string toString() const override
    {
        std::stringstream ss;
        ss for(const auto& path : m_filePaths) { ss << path.string() << "\n"; }
        return ss.str();
    }
#endif

  private:
    std::vector<std::filesystem::path> m_filePaths;
};

}