//
//  BoundingBox.cpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/15.
//

#include "BoundingBox.h"

namespace Lutra {

    BoundingBox::BoundingBox()
    : m_min(std::numeric_limits<float>::max())
    , m_max(-std::numeric_limits<float>::max())
    {

    }

    BoundingBox::BoundingBox(const glm::vec3 &min, const glm::vec3 &max)
    : m_min(min)
    , m_max(max)
    {
    }

    BoundingBox::~BoundingBox()
    {

    }

    void BoundingBox::ExpandBy(const glm::vec3 &point)
    {
        if (point.x < m_min.x) m_min.x = point.x;
        if (point.y < m_min.y) m_min.y = point.y;
        if (point.z < m_min.z) m_min.z = point.z;
        if (m_max.x < point.x) m_max.x = point.x;
        if (m_max.y < point.y) m_max.y = point.y;
        if (m_max.z < point.z) m_max.z = point.z;
    }

    void BoundingBox::ExpandBy(const glm::vec4 &point)
    {
        glm::vec3 value{point.x / point.w, point.y / point.w, point.z / point.w};
        ExpandBy(value);
    }

    void BoundingBox::ExpandBy(const BoundingBox &bbox)
    {
        if (bbox.m_min.x < m_min.x) m_min.x = bbox.m_min.x;
        if (bbox.m_min.y < m_min.y) m_min.y = bbox.m_min.y;
        if (bbox.m_min.z < m_min.z) m_min.z = bbox.m_min.z;
        if (m_max.x < bbox.m_max.x) m_max.x = bbox.m_max.x;
        if (m_max.y < bbox.m_max.y) m_max.y = bbox.m_max.y;
        if (m_max.z < bbox.m_max.z) m_max.z = bbox.m_max.z;
    }

    glm::vec3 BoundingBox::Center() const
    {
        return (m_min + m_max) * 0.5f;
    }

    void BoundingBox::Dirty()
    {
        m_min = glm::vec3(std::numeric_limits<float>::max());
        m_max = glm::vec3(-std::numeric_limits<float>::max());
    }

    bool BoundingBox::IsNull() const
    {
        return m_min == glm::vec3(std::numeric_limits<float>::max()) && m_max == glm::vec3(-std::numeric_limits<float>::max());
    }

}
