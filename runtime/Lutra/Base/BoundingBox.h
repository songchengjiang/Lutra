//
//  BoundingBox.hpp
//  Lutra
//
//  Created by JasonCheng on 2021/1/15.
//

#ifndef BoundingBox_hpp
#define BoundingBox_hpp
#include <glm.hpp>
#include <limits>

namespace Lutra {

    class BoundingBox
    {
    public:
        BoundingBox();
        BoundingBox(const glm::vec3 &min, const glm::vec3 &max);
        ~BoundingBox();

        void ExpandBy(const glm::vec3 &point);
        void ExpandBy(const glm::vec4 &point);
        void ExpandBy(const BoundingBox &bbox);
        
        const glm::vec3& Min() const { return m_min; }
        glm::vec3& Min() { return m_min; }
        const glm::vec3& Max() const { return m_max; }
        glm::vec3& Max() { return m_max; }
        glm::vec3 Center() const;
        
        void Dirty();
        bool IsNull() const;

        inline bool Intersects(const BoundingBox& bbox) const
        {
            if (this->IsNull() || bbox.IsNull())
                return false;

            if (m_max.x < bbox.m_min.x)
                return false;
            if (m_max.y < bbox.m_min.y)
                return false;
            if (m_max.z < bbox.m_min.z)
                return false;

            if (m_min.x > bbox.m_max.x)
                return false;
            if (m_min.y > bbox.m_max.y)
                return false;
            if (m_min.z > bbox.m_max.z)
                return false;

            return true;

        }

        inline BoundingBox operator * (const glm::mat4& m) const {
            BoundingBox bbox;
            bbox.ExpandBy(m * glm::vec4(m_min, 1.0));
            bbox.ExpandBy(m * glm::vec4(m_max.x, m_min.y, m_min.z, 1.0));
            bbox.ExpandBy(m * glm::vec4(m_max.x, m_max.y, m_min.z, 1.0));
            bbox.ExpandBy(m * glm::vec4(m_min.x, m_max.y, m_min.z, 1.0));

            bbox.ExpandBy(m * glm::vec4(m_max, 1.0));
            bbox.ExpandBy(m * glm::vec4(m_min.x, m_max.y, m_max.z, 1.0));
            bbox.ExpandBy(m * glm::vec4(m_min.x, m_min.y, m_max.z, 1.0));
            bbox.ExpandBy(m * glm::vec4(m_max.x, m_min.y, m_max.z, 1.0));
            return bbox;
        }

    private:

        glm::vec3 m_min;
        glm::vec3 m_max;
    };

}

#endif /* BoundingBox_hpp */
